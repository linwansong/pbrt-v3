#if defined(_MSC_VER)
#define NOMINMAX
#pragma once
#endif

#ifndef PBRT_ACCELERATORS_AACACCEL_H
#define PBRT_ACCELERATORS_AACACCEL_H

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

namespace pbrt {

// AACAccel Declarations
struct AACPrimitiveInfo;
struct AACBuildNode;
struct AACThread;

class AACAccel : public Aggregate {
  public:
    // AACAccel Public Methods
    AACAccel(std::vector<std::shared_ptr<Primitive>> p, int delta, Float alpha);
    Bounds3f WorldBound() const { return bounds; }
    ~AACAccel();
    bool Intersect(const Ray &ray, SurfaceInteraction *isect) const;
    bool IntersectP(const Ray &ray) const;

  private:
    // AACAccel Private Methods
    void buildTree(std::vector<MortonPrimitive> mortonPrims);
    void threadBuild(AACThread *th, int start, int startTri, int endTri,
                     int digit, int &finalLen);
    void setLeaf(AACThread *th, int start, int startTri, int numTri);
    void AAClomerate(AACThread *th, int start, int startNum, int endNum,
                     int &finalNum);
    void merge(AACThread *th, int start, int len1, int len2);
    void pruneTree();

    // inline functions
    int inline f(int len) {
        return int(minSize * pow(float(len) / minSize / 2, 0.5 - alpha) -
                   epsi) +
               1;
    };

    float inline SA(Bounds3f a, Bounds3f b) {
        float x = max(a[0].max, b[0].max) - min(a[0].min, b[0].min);
        float y = max(a[1].max, b[1].max) - min(a[1].min, b[1].min);
        float z = max(a[2].max, b[2].max) - min(a[2].min, b[2].min);
        return x * y + x * z + y * z;
    };

    // AACAccel Private Data
    std::vector<std::shared_ptr<Primitive>> primitives;
    Bounds3f bounds;

    const int minSize;
    const double alpha;
    int table[100];

    int *set, *setTmp;
    long long *code;
    int MortonDigit;

    int *lChild, *rChild, *nodeNum, *triNum;
    int curNode;
    bool *isLeaf;
    double *cost, *surA;
};

std::shared_ptr<AACAccel> CreateAACAccelerator(
    std::vector<std::shared_ptr<Primitive>> prims, const ParamSet &ps);

}  // namespace pbrt

#endif  // PBRT_ACCELERATORS_AACACCEL_H
