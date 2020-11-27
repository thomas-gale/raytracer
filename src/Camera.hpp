#ifndef CAMERA_H
#define CAMERA_H

#include "Common.hpp"

namespace raytrace {

template <class T> class Camera {
  public:
    Camera() {
        const auto aspectRatio = 16.0 / 9.0;
        auto viewportHeight = 2.0;
        auto viewportWidth = aspectRatio * viewportHeight;
        auto focalLength = 1.0;

        origin = Point3<double>(0.0, 0.0, 0.0);
        horizontal = Vec3<double>(viewportWidth, 0.0, 0.0);
        vertical = Vec3<double>(0.0, viewportHeight, 0.0);
        lowerLeftCorner = origin - horizontal / 2 - vertical / 2 -
                          Vec3<double>(0.0, 0.0, focalLength);
    }

    Ray<T> getRay(T u, T v) const {
        return Ray<T>(origin,
                      lowerLeftCorner + u * horizontal + v * vertical - origin);
    }

  private:
    Point3<T> origin;
    Point3<T> lowerLeftCorner;
    Vec3<T> horizontal;
    Vec3<T> vertical;
};

} // namespace raytrace

#endif // CAMERA_H