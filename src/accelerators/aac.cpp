
/*
    pbrt source code is Copyright(c) 1998-2016
                        Matt Pharr, Greg Humphreys, and Wenzel Jakob.

    This file is part of pbrt.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are
    met:

    - Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.

    - Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
    IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
    TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
    PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
    HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
    SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
    LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
    DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
    THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

 */

// accelerators/aac.cpp*
#include "accelerators/aac.h"

#include <math.h>

#include <algorithm>

#include "interaction.h"
#include "parallel.h"
#include "paramset.h"
#include "stats.h"

namespace pbrt {

STAT_MEMORY_COUNTER("Memory/AAC tree", treeBytes);
/*
Default 1 prim per leaf node
STAT_RATIO("AAC/Primitives per leaf node", totalPrimitives, totalLeafNodes);
*/
STAT_COUNTER("AAC/Interior nodes", interiorNodes);
STAT_COUNTER("AAC/Leaf nodes", leafNodes);
// struct
struct AACPrimitiveInfo {
    AACPrimitiveInfo() {}
    AACPrimitiveInfo(size_t primitiveNumber, const Bounds3f &bounds)
        : primitiveNumber(primitiveNumber),
          bounds(bounds),
          centroid(.5f * bounds.pMin + .5f * bounds.pMax) {}
    size_t primitiveNumber;
    Bounds3f bounds;
    Point3f centroid;
};

struct AACTreeNode {
    void initLeaf(int index, const Bounds3f &b) {
        primIndex = index;
        isLeaf = true;
        bounds = b;
        children[0] = children[1] = nullptr;
        ++leafNodes;
        //++totalLeafNodes;
    }
    void initInterior(int axis, AACTreeNode *c0, AACTreeNode *c1) {
        children[0] = c0;
        children[1] = c1;
        bounds = Union(c0->bounds, c1->bounds);
        splitAxis = axis;
        isLeaf = false;
        ++interiorNodes;
    }
    Bounds3f bounds;
    AACTreeNode *children[2];
    int splitAxis, primIndex;
    bool isLeaf;
};

struct AACFlatTreeNode {
    Bounds3f bounds;
    union {
        uint32_t primIndex;
        uint32_t rightOffset;  // if -1 means leaf
        uint8_t axis;          // 0 = x, 1 = y, z = 2
    };
};

struct AACCluster {
    AACCluster(AACTreeNode *tn, const Bounds3f &bounds)
        : bounds(bounds),
          centroid(.5f * bounds.pMin + .5f * bounds.pMax),
          treeNode(tn),
          closest(-1),
          distanceToClosest(0) {}

    AACCluster(int axis, AACTreeNode *node, const AACCluster left,
               AACCluster right)
        : bounds(Union(left.bounds, right.bounds)),
          centroid(.5f * bounds.pMin + .5f * bounds.pMax),
          treeNode(node),
          closest(-1),
          distanceToClosest(0) {
        treeNode->initInterior(axis, left.treeNode, right.treeNode);
    }

    Bounds3f bounds;
    Point3f centroid;
    AACTreeNode *treeNode;
    int closest;
    Float distanceToClosest;
};

// AACAccel Method Definitions
AACAccel::AACAccel(std::vector<std::shared_ptr<Primitive>> p, int delta,
                   float alpha)
    : minSize(delta /* / 2 */),
      alpha(alpha),
      MortonDigit(9 + ((p.size() >> 18) > 0)),
      primitives(std::move(p)) {
    ProfilePhase _(Prof::AccelConstruction);
    if (primitives.empty()) return;

    std::vector<AACPrimitiveInfo> primitiveInfo(primitives.size());
    for (size_t i = 0; i < primitives.size(); ++i)
        primitiveInfo[i] = {i, primitives[i]->WorldBound()};

    // Build AAC tree for primitives using _primitiveInfo_
    MemoryArena arena(1024 * 1024);
    int totalNodes = 0;
    std::vector<std::shared_ptr<Primitive>> orderedPrims;
    orderedPrims.reserve(primitives.size());

    Bounds3f bounds;
    for (const AACPrimitiveInfo &pi : primitiveInfo)
        bounds = Union(bounds, pi.centroid);

    // Compute Morton indices of primitives
    std::vector<MortonPrimitive> mortonPrims(primitiveInfo.size());
    ParallelFor(
        [&](int i) {
            // Initialize _mortonPrims[i]_ for _i_th primitive
            PBRT_CONSTEXPR int mortonBits = 10;
            PBRT_CONSTEXPR int mortonScale = 1 << mortonBits;
            mortonPrims[i].primitiveIndex = primitiveInfo[i].primitiveNumber;
            Vector3f centroidOffset = bounds.Offset(primitiveInfo[i].centroid);
            mortonPrims[i].mortonCode =
                EncodeMorton3(centroidOffset * mortonScale);
        },
        primitiveInfo.size(), 512);

    // Radix sort primitive Morton indices
    RadixSort(&mortonPrims);

    // Start recursive construction of aac tree
    AACTreeNode *root =
        buildTree(arena, mortonPrims, orderedPrims, primitiveInfo, &totalNodes);

    primitives.swap(orderedPrims);
    primitiveInfo.resize(0);
    LOG(INFO) << StringPrintf(
        "AAC created with %d nodes for %d "
        "primitives (%.2f MB), arena allocated %.2f MB",
        totalNodes, (int)primitives.size(),
        float(totalNodes * sizeof(AACFlatTreeNode)) / (1024.f * 1024.f),
        float(arena.TotalAllocated()) / (1024.f * 1024.f));

    // Compute representation of depth-first traversal of AAC tree
    treeBytes += totalNodes * sizeof(AACFlatTreeNode) + sizeof(*this) +
                 primitives.size() * sizeof(primitives[0]);
    nodes = AllocAligned<AACFlatTreeNode>(totalNodes);
    int offset = 0;
    flattenAACTree(root, &offset);
    CHECK_EQ(totalNodes, offset);
}

AACAccel::~AACAccel() { FreeAligned(nodes); }

// AACAccel Private Methods
AACTreeNode *AACAccel::buildTree(
    MemoryArena &arena, std::vector<MortonPrimitive> &mortonPrims,
    std::vector<std::shared_ptr<Primitive>> &orderedPrims,
    std::vector<AACPrimitiveInfo> &primitiveInfo, int *totalNodes) {
    std::vector<AACCluster> lastCluster =
        buildTree(arena, mortonPrims, orderedPrims, primitiveInfo, totalNodes,
                  0, mortonPrims.size());
    combineClusters(arena, lastCluster, totalNodes, 1, 3);
    this->bounds = lastCluster[0].bounds;
    return lastCluster[0].treeNode;
}

// [left, right)
std::vector<AACCluster> AACAccel::buildTree(
    MemoryArena &arena, std::vector<MortonPrimitive> &mortonPrims,
    std::vector<std::shared_ptr<Primitive>> &orderedPrims,
    std::vector<AACPrimitiveInfo> &primitiveInfo, int *totalNodes, int left,
    int right) {
    // Initialize new Cluster
    std::vector<AACCluster> cluster{};

    if (right - left < minSize) {
        // Leaf Nodes
        std::vector<AACCluster> leafNodes();
        for (int i = left; i < right; i++) {
            orderedPrims.push_back(
                primitives[primitiveInfo[i].primitiveNumber]);
            AACTreeNode *node = arena.Alloc<AACTreeNode>();
            node->initLeaf(i, primitiveInfo[i].bounds);
            *totalNodes++;
            cluster.push_back(AACCluster{node, primitiveInfo[i].bounds});
        }
        combineClusters(arena, cluster, totalNodes, f(minSize), 3);
    } else {
        // Interior Nodes
        int axis = 0;
        int part = makePartition(mortonPrims, left, right, &axis);
        cluster = buildTree(arena, mortonPrims, orderedPrims, primitiveInfo,
                            totalNodes, left, part);
        std::vector<AACCluster> rightCluster =
            buildTree(arena, mortonPrims, orderedPrims, primitiveInfo,
                      totalNodes, part, right);
        cluster.insert(cluster.end(), rightCluster.begin(), rightCluster.end());
        combineClusters(arena, cluster, totalNodes, f(right - left), axis);
    }
    return cluster;
}

// Partition
int AACAccel::makePartition(std::vector<MortonPrimitive> &mortonPrims, int left,
                            int right, int *axis) {
    // Took Morton Digit splitting code from paper
    int digit = MortonDigit * 3 - 1;
    int start = left, end = right - 1, mid;
    // digit used to find place to split Morton Code
    uint32_t mask = (uint32_t)1 << digit;
    while (digit >= 0) {
        if (!(mortonPrims[start].mortonCode & mask) &&
            (mortonPrims[end].mortonCode & mask))
            break;
        digit--;
        mask = (uint32_t)1 << digit;
    }

    if (digit < 0) {
        // if the Morton code split wasn't enough, split set into 2
        start = (start + end) >> 1;
        *axis = 3;
    } else {
        while (start < end) {
            // binary search to find
            mid = (start + end) >> 1;
            if (mortonPrims[mid].mortonCode & mask)
                end = mid;
            else
                start = mid + 1;
        }
        // 0 = x, 1 = y, 2 = z, 3 = none
        *axis = ((int)std::log2(digit)) % 3;
    }
    return start;
}

// Find best match
void AACAccel::findBestMatch(int i, std::vector<AACCluster> &cluster) {
    int minDist = infi;
    for (int j = 0; j < cluster.size(); j++) {
        if (j != i) {
            int d = Distance(cluster[i].centroid, cluster[j].centroid);
            if (d < minDist) {
                cluster[i].closest = j;
                cluster[i].distanceToClosest = d;
            }
        }
    }
}

void AACAccel::combineClusters(MemoryArena &arena,
                               std::vector<AACCluster> &cluster,
                               int *totalNodes, int n, int axis) {
    for (int i = 0; i < cluster.size(); i++) {
        findBestMatch(i, cluster);
    }

    while (cluster.size() > n) {
        Float best = infi;
        int left, right;
        for (int i = 0; i < cluster.size(); i++) {
            if (cluster[i].distanceToClosest < best) {
                best = cluster[i].distanceToClosest;
                left = std::min(i, cluster[i].closest);
                right = std::max(i, cluster[i].closest);
            }
        }
        AACTreeNode *node = arena.Alloc<AACTreeNode>();
        AACCluster nCluster{axis, node, cluster[left], cluster[right]};
        *totalNodes++;

        // erase right first so not to mess up indexing
        cluster.erase(cluster.begin() + right);
        cluster.erase(cluster.begin() + left);
        cluster.push_back(nCluster);
        findBestMatch(cluster.size() - 1, cluster);  // new cluster at end
        for (int i = 0; i < cluster.size(); i++) {
            if (cluster[i].closest == left || cluster[i].closest == right) {
                findBestMatch(i, cluster);
            }
            if (cluster[i].closest <= right) cluster[i].closest--;
            if (cluster[i].closest <= left) cluster[i].closest--;
        }
    }
}

int AACAccel::flattenAACTree(AACTreeNode *node, int *offset) {
    AACFlatTreeNode *linearNode = &nodes[*offset];
    linearNode->bounds = node->bounds;
    int myOffset = (*offset)++;
    if (!node->isLeaf) {
        CHECK(!node->children[0] && !node->children[1]);
        linearNode->primIndex = node->primIndex;
        linearNode->rightOffset = -1;
    } else {
        // Create interior flattened AAC node
        linearNode->axis = node->splitAxis;
        linearNode->primIndex = flattenAACTree(node->children[0], offset);
        linearNode->rightOffset = flattenAACTree(node->children[1], offset);
    }
    return myOffset;
}

int AACAccel::f(int len) {
    return int(minSize * pow(float(len) / minSize / 2, 0.5 - alpha) - epsi) + 1;
}

bool AACAccel::Intersect(const Ray &ray, SurfaceInteraction *isect) const {
    if (!nodes) return false;
    ProfilePhase p(Prof::AccelIntersect);
    bool hit = false;
    Vector3f invDir(1 / ray.d.x, 1 / ray.d.y, 1 / ray.d.z);
    int dirIsNeg[3] = {invDir.x < 0, invDir.y < 0, invDir.z < 0};
    // Follow ray through AAC nodes to find primitive intersections
    int toVisitOffset = 0, currentNodeIndex = 0;
    int nodesToVisit[64];
    while (true) {
        const AACFlatTreeNode *node = &nodes[currentNodeIndex];
        // Check ray against AAC node
        if (node->bounds.IntersectP(ray, invDir, dirIsNeg)) {
            if (node->rightOffset < 0) {
                // Intersect ray with primitives in leaf AAC node
                if (primitives[node->primIndex]->Intersect(ray, isect))
                    hit = true;
                if (toVisitOffset == 0) break;
                currentNodeIndex = nodesToVisit[--toVisitOffset];
            } else {
                if (node->axis < 3 && dirIsNeg[node->axis]) {
                    nodesToVisit[toVisitOffset++] = currentNodeIndex + 1;
                    currentNodeIndex = node->rightOffset;
                } else {
                    nodesToVisit[toVisitOffset++] = node->rightOffset;
                    currentNodeIndex = currentNodeIndex + 1;
                }
            }
        } else {
            if (toVisitOffset == 0) break;
            currentNodeIndex = nodesToVisit[--toVisitOffset];
        }
    }
    return hit;
}

bool AACAccel::IntersectP(const Ray &ray) const {
    if (!nodes) return false;
    ProfilePhase p(Prof::AccelIntersectP);
    Vector3f invDir(1.f / ray.d.x, 1.f / ray.d.y, 1.f / ray.d.z);
    int dirIsNeg[3] = {invDir.x < 0, invDir.y < 0, invDir.z < 0};
    int nodesToVisit[64];
    int toVisitOffset = 0, currentNodeIndex = 0;
    while (true) {
        const AACFlatTreeNode *node = &nodes[currentNodeIndex];
        if (node->bounds.IntersectP(ray, invDir, dirIsNeg)) {
            // Process AAC node _node_ for traversal
            if (node->rightOffset > 0) {
                if (primitives[node->primIndex]->IntersectP(ray)) return true;
                if (toVisitOffset == 0) break;
                currentNodeIndex = nodesToVisit[--toVisitOffset];
            } else {
                if (node->axis < 3 && dirIsNeg[node->axis]) {
                    /// second child first
                    nodesToVisit[toVisitOffset++] = currentNodeIndex + 1;
                    currentNodeIndex = node->rightOffset;
                } else {
                    nodesToVisit[toVisitOffset++] = node->rightOffset;
                    currentNodeIndex = currentNodeIndex + 1;
                }
            }
        } else {
            if (toVisitOffset == 0) break;
            currentNodeIndex = nodesToVisit[--toVisitOffset];
        }
    }
    return false;
}

std::shared_ptr<AACAccel> CreateAACAccelerator(
    std::vector<std::shared_ptr<Primitive>> prims, const ParamSet &ps) {
    int delta = ps.FindOneInt("aacdelta", 20);
    Float alpha = ps.FindOneFloat("aacalpha", 0.1f);
    return std::make_shared<AACAccel>(std::move(prims), delta, alpha);
}

}  // namespace pbrt
