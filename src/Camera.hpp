#ifndef CAMERA_H
#define CAMERA_H

#include "Common.hpp"

namespace raytrace {

template <class T> class Camera {
  public:
    Camera(Point3<T> lookFrom, Point3<T> lookAt, Vec3<T> vUp, T vFovDeg,
           T aspectRatio, T aperture, T focusDist) {
        auto theta = degToRad(vFovDeg);
        auto h = std::tan(theta / 2);
        auto viewportHeight = 2.0 * h;
        auto viewportWidth = aspectRatio * viewportHeight;

        // Orthonormalise from vUp and lookAt/lookFrom.
        w = unit(lookFrom - lookAt);
        u = unit(cross(vUp, w));
        v = cross(w, u);

        origin = lookFrom;
        horizontal = focusDist * viewportWidth * u;
        vertical = focusDist * viewportHeight * v;
        lowerLeftCorner =
            origin - horizontal / 2 - vertical / 2 - focusDist * w;
        lensRadius = aperture / 2;
    }

    Ray<T> getRay(T s, T t) const {
        // Sample a ray from within the lens radius
        Vec3<T> rd = lensRadius * Vec3<T>::randomInUnitDisk();
        Vec3<T> offset = u * rd.x() + v * rd.y();

        return Ray<T>(origin + offset, lowerLeftCorner + s * horizontal +
                                           t * vertical - origin - offset);
    }

  private:
    Point3<T> origin;
    Point3<T> lowerLeftCorner;
    Vec3<T> horizontal;
    Vec3<T> vertical;
    Vec3<T> u, v, w;
    T lensRadius;
};

} // namespace raytrace

#endif // CAMERA_H