#ifndef RAY_H
#define RAY_H

#include "Vec3.hpp"

namespace raytrace {

template <class T> class Ray {
  public:
    Ray() {}
    Ray(const Point3<T>& origin, const Vec3<T>& direction)
        : orig(origin), dir(direction) {}

    Point3<T> origin() const { return orig; }
    Vec3<T> direction() const { return dir; }

    Point3<T> at(T t) const { return orig + t * dir; }

  private:
    Point3<T> orig;
    Vec3<T> dir;
};

} // namespace raytrace

#endif // RAY_H