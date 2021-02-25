
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

// accelerators/AACaccel.cpp*
#include "accelerators/aac.h"

#include <algorithm>

#include "interaction.h"
#include "paramset.h"
#include "stats.h"
#include "parallel.h"
#include "accelerators/morton.h"

namespace pbrt {

STAT_MEMORY_COUNTER("Memory/AAC tree", treeBytes);
STAT_RATIO("AAC/Primitives per leaf node", totalPrimitives, totalLeafNodes);
STAT_COUNTER("AAC/Interior nodes", interiorNodes);
STAT_COUNTER("AAC/Leaf nodes", leafNodes);

// AACAccel Local Declarations
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

struct AACBuildNode {
    // AACBuildNode Public Methods
    void InitLeaf(int first, int n, const Bounds3f &b) {
        firstPrimOffset = first;
        nPrimitives = n;
        bounds = b;
        children[0] = children[1] = nullptr;
        ++leafNodes;
        ++totalLeafNodes;
        totalPrimitives += n;
    }
    void InitInterior(int axis, AACBuildNode *c0, AACBuildNode *c1) {
        children[0] = c0;
        children[1] = c1;
        bounds = Union(c0->bounds, c1->bounds);
        splitAxis = axis;
        nPrimitives = 0;
        ++interiorNodes;
    }
    Bounds3f bounds;
    AACBuildNode *children[2];
    int splitAxis, firstPrimOffset, nPrimitives;
};

struct AACThread {
    float **area, *minArea;
    int *label, *minPos, *minLabel;
    int size;
    void init(int size) {
        this->size = size;
        label = new int[size];
        minArea = new float[size];
        minPos = new int[size];
        minLabel = new int[size];
        area = new float *[size];
        for (int i = 0; i < size; i++) area[i] = new float[size];
    }
    ~AACThread() {
        delete[] label;
        delete[] minArea;
        delete[] minPos;
        delete[] minLabel;
        for (int i = 0; i < size; i++) delete[] area[i];
        delete[] area;
    }
};

// AACAccel Method Definitions
AACAccel::AACAccel(std::vector<std::shared_ptr<Primitive>> p, int delta,
                   Float alpha)
    : minSize(delta / 2), alpha(alpha),
      primitives(std::move(p))  {
    ProfilePhase _(Prof::AccelConstruction);
    if (primitives.empty()) return;
    // Build with AAC from _primitives_

    // no idea what this is for
    for (int i = 0; i < 100; i++)
        table[i] =
            int(delta * pow(float(i) / delta, 0.5 - alpha) / 2 - epsi) + 1;
    
    std::vector<AACPrimitiveInfo> primitiveInfo(primitives.size());
    for (size_t i = 0; i < primitives.size(); ++i)
        primitiveInfo[i] = {i, primitives[i]->WorldBound()};
    
    // Build BVH tree for primitives using _primitiveInfo_
/*     int totalNodes = 0; */
    std::vector<std::shared_ptr<Primitive>> orderedPrims;
    orderedPrims.reserve(primitives.size());
    AACBuildNode *root;

    // Compute bounding box of all primitive centroids
    Bounds3f bounds;
    for (const AACPrimitiveInfo &pi : primitiveInfo)
        bounds = Union(bounds, pi.centroid);

    // Compute Morton indices of primitives
    std::vector<MortonPrimitive> mortonPrims(primitiveInfo.size());
    ParallelFor([&](int i) {
        // Initialize _mortonPrims[i]_ for _i_th primitive
        PBRT_CONSTEXPR int mortonBits = 10;
        PBRT_CONSTEXPR int mortonScale = 1 << mortonBits;
        mortonPrims[i].primitiveIndex = primitiveInfo[i].primitiveNumber;
        Vector3f centroidOffset = bounds.Offset(primitiveInfo[i].centroid);
        mortonPrims[i].mortonCode = EncodeMorton3(centroidOffset * mortonScale);
    }, primitiveInfo.size(), 512);

    // Radix sort primitive Morton indices
    RadixSort(&mortonPrims);

    code = new long long[primitives.size()];
    set = new int[primitives.size()];
    setTmp = new int[primitives.size()];
    cost = new double[primitives.size() * 2];
    surA = new double[primitives.size() * 2];
    isLeaf = new bool[primitives.size() * 2];
    nodeNum = new int[primitives.size() * 2];

    // Start recursive construction of aac tree
    buildTree();
}

AACAccel::~AACAccel() {
    delete[] set;
    delete[] code;
    delete[] setTmp;
    delete[] nodeNum;
    delete[] triNum;
    delete[] lChild;
    delete[] rChild;
    delete[] AABB;
    delete[] isLeaf;
    delete[] cost;
}

void AACAccel::buildTree() {
    curNode = primitives.size();
    AACThread th;
    th.init(4 *
            int(minSize * pow(primitives.size() / 2.0 / minSize, 0.5 - alpha / 2) +
                1e-5));

    int finalLen = 0;
    threadBuild(&th, 0, 0, primitives.size(), MortonDigit * 3 - 1, finalLen);
    AAClomerate(&th, 0, finalLen, 1, finalLen);

    pruneTree();
}

// A single agglomerative clustering thread.
void AACAccel::threadBuild(AACThread *th, int start, int startTri, int endTri, int digit,
                 int &finalLen) {
    if (startTri >= endTri) {
        finalLen = 0;
        return;
    }
    if (endTri - startTri < minSize * 2) {
        assert(start + endTri - startTri < th->size);
        setLeaf(th, start, startTri, endTri - startTri);
        AAClomerate(th, start, endTri - startTri, minSize, finalLen);
        return;
    }
    int s = startTri, t = endTri - 1, mid;
    long long tar = (long long)1 << digit;
    while (digit >= 0) {
        if (!(code[set[s]] & tar) && (code[set[t]] & tar)) break;
        digit--;
        tar = (long long)1 << digit;
    }

    if (digit < 0)
        s = (s + t) >> 1;
    else
        while (s < t) {
            mid = (s + t) >> 1;
            if (code[set[mid]] & tar)
                t = mid;
            else
                s = mid + 1;
        }
    int len1, len2;
    threadBuild(th, start, startTri, s, digit - 1, len1);
    threadBuild(th, start + len1, s, endTri, digit - 1, len2);
    merge(th, start, len1, len2);
    finalLen = (endTri - startTri >= 100) ? f(endTri - startTri)
                                          : table[endTri - startTri];
    AAClomerate(th, start, len1 + len2, finalLen, finalLen);
}

// Set leaves.
void AACAccel::setLeaf(AACThread *th, int start, int startTri, int numTri) {
    for (int i = 0; i < numTri; i++) {
        th->minArea[start + i] = infi;
        th->label[start + i] = set[startTri + i];
    }
    for (int i = start; i < start + numTri; i++) {
        for (int j = start; j < i; j++) {
            th->area[i][j] = SA(AABB[th->label[i]], AABB[th->label[j]]);
            if (th->area[i][j] < th->minArea[i]) {
                th->minArea[i] = th->area[i][j];
                th->minPos[i] = j;
                th->minLabel[i] = set[j];
            }
            if (th->area[i][j] < th->minArea[j]) {
                th->minArea[j] = th->area[i][j];
                th->minPos[j] = i;
                th->minLabel[j] = set[i];
            }
        }
    }
}

// The function to process agglomeration.
void AACAccel::AAClomerate(AACThread *th, int start, int startNum, int endNum,
                 int &finalNum) {
    int n = startNum;
    finalNum = min(startNum, endNum);
    int i, j, a, b, last = 0;
    while (n > endNum) {
        float mn = infi;
        for (i = start; i < start + n; i++) {
            if (th->minPos[i] == start + n)
                if (last == start + n)
                    th->minLabel[i] = -1;
                else
                    th->minPos[i] = last;
            if (th->minLabel[i] != th->label[th->minPos[i]]) {
                th->minArea[i] = infi;
                for (j = start; j < i; j++)
                    if (th->area[i][j] < th->minArea[i]) {
                        th->minArea[i] = th->area[i][j];
                        th->minPos[i] = j;
                        th->minLabel[i] = th->label[j];
                    }
                for (j = i + 1; j < start + n; j++)
                    if (th->area[j][i] < th->minArea[i]) {
                        th->minArea[i] = th->area[j][i];
                        th->minPos[i] = j;
                        th->minLabel[i] = th->label[j];
                    }
            }
            if (th->minArea[i] < mn) {
                mn = th->minArea[i];
                a = i;
                b = th->minPos[i];
            }
        }
        lChild[curNode] = th->label[a];
        rChild[curNode] = th->label[b];
        update(AABB[curNode], AABB[lChild[curNode]], AABB[rChild[curNode]]);
        triNum[curNode] = triNum[lChild[curNode]] + triNum[rChild[curNode]];
        for (j = start; j < a; j++)
            th->area[a][j] = SA(AABB[curNode], AABB[th->label[j]]);
        for (j = a + 1; j < start + n; j++)
            th->area[j][a] = SA(AABB[curNode], AABB[th->label[j]]);
        n--;
        for (j = start; j < b; j++) th->area[b][j] = th->area[start + n][j];
        for (j = b + 1; j < start + n; j++)
            th->area[j][b] = th->area[start + n][j];
        th->label[a] = curNode;
        th->label[b] = th->label[start + n];
        th->minArea[b] = th->minArea[start + n];
        th->minLabel[b] = th->minLabel[start + n];
        th->minPos[b] = th->minPos[start + n];
        last = b;
        curNode++;
    }
}

// Combine two subtaskes.
void AACAccel::merge(AACThread *th, int start, int len1, int len2) {
    for (int i = start; i < start + len1; i++)
        for (int j = start + len1; j < start + len1 + len2; j++) {
            th->area[j][i] = SA(AABB[th->label[i]], AABB[th->label[j]]);
            if (th->area[j][i] < th->minArea[i]) {
                th->minArea[i] = th->area[j][i];
                th->minPos[i] = j;
                th->minLabel[i] = set[j];
            }
            if (th->area[j][i] < th->minArea[j]) {
                th->minArea[j] = th->area[j][i];
                th->minPos[j] = i;
                th->minLabel[j] = set[i];
            }
        }
}

// Prune tree at last. This can be also done in tree building process.
void AACAccel::pruneTree() {
    for (int i = 0; i < primitives.size(); i++) {
        nodeNum[i] = 1;
        cost[i] = 2;
        surA[i] = SurA(AABB[i]);
        isLeaf[i] = true;
    }
    for (int i = primitives.size(); i < curNode; i++) {
        surA[i] = SurA(AABB[i]);
        assert(SurA(AABB[i]) - SA(AABB[lChild[i]], AABB[rChild[i]]) < 1e-8);
        double tmp = surA[lChild[i]] / surA[i] * cost[lChild[i]] +
                     surA[lChild[i]] / surA[i] * cost[lChild[i]];
        if (tmp + 1 > triNum[i] * 2) {
            cost[i] = triNum[i];
            nodeNum[i] = 1;
            isLeaf[i] = true;
        } else {
            cost[i] = tmp + 1;
            nodeNum[i] = nodeNum[lChild[i]] + nodeNum[rChild[i]] + 1;
            isLeaf[i] = false;
        }
    }
}

bool AACAccel::Intersect(const Ray &ray, SurfaceInteraction *isect) const {
    ProfilePhase p(Prof::AccelIntersect);
    // Compute initial parametric range of ray inside kd-tree extent
    Float tMin, tMax;
    if (!bounds.IntersectP(ray, &tMin, &tMax)) {
        return false;
    }

    // Prepare to traverse kd-tree for ray
    Vector3f invDir(1 / ray.d.x, 1 / ray.d.y, 1 / ray.d.z);
    PBRT_CONSTEXPR int maxTodo = 64;
    KdToDo todo[maxTodo];
    int todoPos = 0;

    // Traverse kd-tree nodes in order for ray
    bool hit = false;
    const KdAccelNode *node = &nodes[0];
    while (node != nullptr) {
        // Bail out if we found a hit closer than the current node
        if (ray.tMax < tMin) break;
        if (!node->IsLeaf()) {
            // Process kd-tree interior node

            // Compute parametric distance along ray to split plane
            int axis = node->SplitAxis();
            Float tPlane = (node->SplitPos() - ray.o[axis]) * invDir[axis];

            // Get node children pointers for ray
            const KdAccelNode *firstChild, *secondChild;
            int belowFirst =
                (ray.o[axis] < node->SplitPos()) ||
                (ray.o[axis] == node->SplitPos() && ray.d[axis] <= 0);
            if (belowFirst) {
                firstChild = node + 1;
                secondChild = &nodes[node->AboveChild()];
            } else {
                firstChild = &nodes[node->AboveChild()];
                secondChild = node + 1;
            }

            // Advance to next child node, possibly enqueue other child
            if (tPlane > tMax || tPlane <= 0)
                node = firstChild;
            else if (tPlane < tMin)
                node = secondChild;
            else {
                // Enqueue _secondChild_ in todo list
                todo[todoPos].node = secondChild;
                todo[todoPos].tMin = tPlane;
                todo[todoPos].tMax = tMax;
                ++todoPos;
                node = firstChild;
                tMax = tPlane;
            }
        } else {
            // Check for intersections inside leaf node
            int nPrimitives = node->nPrimitives();
            if (nPrimitives == 1) {
                const std::shared_ptr<Primitive> &p =
                    primitives[node->onePrimitive];
                // Check one primitive inside leaf node
                if (p->Intersect(ray, isect)) hit = true;
            } else {
                for (int i = 0; i < nPrimitives; ++i) {
                    int index =
                        primitiveIndices[node->primitiveIndicesOffset + i];
                    const std::shared_ptr<Primitive> &p = primitives[index];
                    // Check one primitive inside leaf node
                    if (p->Intersect(ray, isect)) hit = true;
                }
            }

            // Grab next node to process from todo list
            if (todoPos > 0) {
                --todoPos;
                node = todo[todoPos].node;
                tMin = todo[todoPos].tMin;
                tMax = todo[todoPos].tMax;
            } else
                break;
        }
    }
    return hit;
}

bool AACAccel::IntersectP(const Ray &ray) const {
    ProfilePhase p(Prof::AccelIntersectP);
    // Compute initial parametric range of ray inside kd-tree extent
    Float tMin, tMax;
    if (!bounds.IntersectP(ray, &tMin, &tMax)) {
        return false;
    }

    // Prepare to traverse kd-tree for ray
    Vector3f invDir(1 / ray.d.x, 1 / ray.d.y, 1 / ray.d.z);
    PBRT_CONSTEXPR int maxTodo = 64;
    KdToDo todo[maxTodo];
    int todoPos = 0;
    const KdAccelNode *node = &nodes[0];
    while (node != nullptr) {
        if (node->IsLeaf()) {
            // Check for shadow ray intersections inside leaf node
            int nPrimitives = node->nPrimitives();
            if (nPrimitives == 1) {
                const std::shared_ptr<Primitive> &p =
                    primitives[node->onePrimitive];
                if (p->IntersectP(ray)) {
                    return true;
                }
            } else {
                for (int i = 0; i < nPrimitives; ++i) {
                    int primitiveIndex =
                        primitiveIndices[node->primitiveIndicesOffset + i];
                    const std::shared_ptr<Primitive> &prim =
                        primitives[primitiveIndex];
                    if (prim->IntersectP(ray)) {
                        return true;
                    }
                }
            }

            // Grab next node to process from todo list
            if (todoPos > 0) {
                --todoPos;
                node = todo[todoPos].node;
                tMin = todo[todoPos].tMin;
                tMax = todo[todoPos].tMax;
            } else
                break;
        } else {
            // Process kd-tree interior node

            // Compute parametric distance along ray to split plane
            int axis = node->SplitAxis();
            Float tPlane = (node->SplitPos() - ray.o[axis]) * invDir[axis];

            // Get node children pointers for ray
            const KdAccelNode *firstChild, *secondChild;
            int belowFirst =
                (ray.o[axis] < node->SplitPos()) ||
                (ray.o[axis] == node->SplitPos() && ray.d[axis] <= 0);
            if (belowFirst) {
                firstChild = node + 1;
                secondChild = &nodes[node->AboveChild()];
            } else {
                firstChild = &nodes[node->AboveChild()];
                secondChild = node + 1;
            }

            // Advance to next child node, possibly enqueue other child
            if (tPlane > tMax || tPlane <= 0)
                node = firstChild;
            else if (tPlane < tMin)
                node = secondChild;
            else {
                // Enqueue _secondChild_ in todo list
                todo[todoPos].node = secondChild;
                todo[todoPos].tMin = tPlane;
                todo[todoPos].tMax = tMax;
                ++todoPos;
                node = firstChild;
                tMax = tPlane;
            }
        }
    }
    return false;
}

std::shared_ptr<AACAccel> CreateAACAccelerator(
    std::vector<std::shared_ptr<Primitive>> prims, const ParamSet &ps) {
    int delta = ps.FindOneInt("aacdelta", 20);
    Float alpha = ps.FindOneFloat("aacalpha", 0.1f);
    return std::make_shared<AACAccel>(std::move(prims), isectCost, travCost,
                                      emptyBonus, maxPrims, maxDepth);
}

}  // namespace pbrt
