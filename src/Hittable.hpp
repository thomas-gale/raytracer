#ifndef HITTABLE_H
#define HITTABLE_H

#include "Vec3.hpp"

namespace raytrace {

template <class T> class HitRecord {
  public:
    Point3<T> p;
    Vec3<T> normal;
    T t;
    bool frontFace;

    inline void setFaceNormal(const Ray<T>& r, const Vec3<T>& outwardNormal) {
        frontFace = dot(r.direction(), outwardNormal) < 0;
        normal = frontFace ? outwardNormal : -outwardNormal;
    }
};

template <class T> class Hittable {
  public:
    virtual bool hit(const Ray<T>& r, T tMin, T tMax,
                     HitRecord<T>& rec) const = 0;
};

} // namespace raytrace

#endif // HITTABLE_H