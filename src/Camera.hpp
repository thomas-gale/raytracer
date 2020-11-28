#ifndef CAMERA_H
#define CAMERA_H

#include "Common.hpp"

namespace raytrace {

template <class T> class Camera {
  public:
    Camera(Point3<T> lookFrom, Point3<T> lookAt, Vec3<T> vUp, T vFovDeg,
           T aspectRatio) {
        auto theta = degToRad(vFovDeg);
        auto h = std::tan(theta / 2);
        auto viewportHeight = 2.0 * h;
        auto viewportWidth = aspectRatio * viewportHeight;

        // Orthonormalise from vUp and lookAt/lookFrom.
        auto w = unit(lookFrom - lookAt);
        auto u = unit(cross(vUp, w));
        auto v = cross(w, u);

        auto focalLength = 1.0;

        origin = lookFrom;
        horizontal = viewportWidth * u;
        vertical = viewportHeight * v;
        lowerLeftCorner = origin - horizontal / 2 - vertical / 2 - w;
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