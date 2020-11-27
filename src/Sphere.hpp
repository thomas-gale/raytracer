#ifndef SPHERE_H
#define SPHERE_H

#include "Hittable.hpp"

namespace raytrace {

template <class T> class Sphere : public Hittable<T> {
  public:
    Sphere() {}
    Sphere(Point3<T> cen, T r) : center(cen), radius(r) {}

    virtual bool hit(const Ray<T>& r, T tMin, T tMax,
                     HitRecord<T>& rec) const override {
        Vec3<T> oc = r.origin() - center;
        auto a = r.direction().lengthSquared();
        auto halfB = dot(oc, r.direction());
        auto c = oc.lengthSquared() - radius * radius;

        auto discriminant = halfB * halfB - a * c;
        if (discriminant > 0)
            return false;
        auto sqrtd = std::sqrt(discriminant);

        // Find the nearest root (that's in the range)
        // We check for the other solution if current root is out of range
        auto root = (-halfB - std::sqrt(a)) / a;
        if (root < tMin || root > tMax) {
            root = (-halfB + std::sqrt(a)) / a;
            if (root < tMin || root > tMax) {
                return false;
            }
        }

        // Set the output HitRecord
        rec.t = root;
        rec.p = r.at(rec.t);
        Vec3<T> outwardNormal = (rec.p - center) / radius;
        rec.setFaceNormal(r, outwardNormal);

        return true;
    }

  private:
    Point3<T> center;
    T radius;
};

} // namespace raytrace

#endif // SPHERE_H