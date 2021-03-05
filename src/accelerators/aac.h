#if defined(_MSC_VER)
#define NOMINMAX
#pragma once
#endif

#ifndef PBRT_ACCELERATORS_AACACCEL_H
#define PBRT_ACCELERATORS_AACACCEL_H
<<<<<<< HEAD
#include <atomic>

#include "accelerators/morton.h"
#include "pbrt.h"
#include "primitive.h"

// accelerators/acc.h*
=======

// accelerators/kdtreeaccel.h*
#include "pbrt.h"
#include "primitive.h"
#include "accelerators/morton.h"

#define epsi 1e-8
#define sqr(a) (a) * (a)
#define set(a, b) memmove((a), (b), sizeof(Bounds3f))
#define setPoint(a, b) memmove((a), (b), sizeof(Point3D))
#define SurA(a) ((a[0].max - a[0].min) * (a[1].max - a[1].min) + (a[0].max - a[0].min) * (a[2].max - a[2].min) + (a[1].max - a[1].min) * (a[2].max - a[2].min))
const float infi = 1e10;

>>>>>>> deb9d25095a00d671f20fe376c6fcbd785e6ed82
namespace pbrt {

// AACAccel Declarations
struct AACPrimitiveInfo;
struct AACTreeNode;
struct AACFlatTreeNode;
struct AACCluster;

class AACAccel : public Aggregate {
  public:
    // AACAccel Public Methods
    AACAccel(std::vector<std::shared_ptr<Primitive>> p, int delta, float alpha);
    ~AACAccel();
    Bounds3f WorldBound() const { return bounds; };
    bool Intersect(const Ray &ray, SurfaceInteraction *isect) const;
    bool IntersectP(const Ray &ray) const;

  private:
    // AACAccel Private Methods
    AACTreeNode *buildTree(
        MemoryArena &arena, std::vector<MortonPrimitive> &mortonPrims,
        std::vector<std::shared_ptr<Primitive>> &orderedPrims,
        std::vector<AACPrimitiveInfo> &primitiveInfo, int *totalNodes);

    std::vector<AACCluster> buildTree(
        MemoryArena &arena, std::vector<MortonPrimitive> &mortonPrims,
        std::vector<std::shared_ptr<Primitive>> &orderedPrims,
        std::vector<AACPrimitiveInfo> &primitiveInfo, int *totalNodes, int left,
        int right);

    // Partition
    int makePartition(std::vector<MortonPrimitive> &mortonPrims, int left,
                      int right, int *axis);
    // Find best match for cluster i
    void findBestMatch(int i, std::vector<AACCluster> &cluster);
    void combineClusters(
        MemoryArena &arena, std::vector<AACCluster> &cluster, int *totalNodes, int n, int axis);

    int flattenAACTree(AACTreeNode *node, int *offset);
    // inline functions
    int f(int len);

    // AACAccel Private Data
    const int minSize;
    const double alpha;
    const int MortonDigit;

    const float epsi = 1e-8;
    const float infi = 1e10;

    std::vector<std::shared_ptr<Primitive>> primitives;
    Bounds3f bounds;
    AACFlatTreeNode *nodes = nullptr;
};

std::shared_ptr<AACAccel> CreateAACAccelerator(
    std::vector<std::shared_ptr<Primitive>> prims, const ParamSet &ps);

}  // namespace pbrt

#endif  // PBRT_ACCELERATORS_AACACCEL_H
