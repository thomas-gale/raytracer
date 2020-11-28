#ifndef MATERIAL_H
#define MATERIAL_H

#include "Common.hpp"

namespace raytrace {

// Abstract class for materials
template <class T> class Material {
  public:
    // Returns if the ray is absorbed, optionally outputs scatter ray with
    // attenuation information.
    virtual bool scatter(const Ray<T>& rIn, const HitRecord<T>& rec,
                         Color<T>& attenuation, Ray<T>& scattered) const = 0;
};

// Lambertian (diffuse) material.
template <class T> class Lambertian : public Material<T> {
  public:
    Lambertian(const Color<T>& a) : albedo(a) {}

    virtual bool scatter(const Ray<T>& rIn, const HitRecord<T>& rec,
                         Color<T>& attenuation,
                         Ray<T>& scattered) const override {
        auto scatterDirection = rec.normal + Vec3<T>::randomUnitVec();

        // Catch degenerate scatter direction
        if (scatterDirection.nearZero())
            scatterDirection = rec.normal;

        scattered = Ray<T>(rec.p, scatterDirection);
        attenuation = albedo;
        return true;
    }

  private:
    Color<T> albedo;
};

// Metal (reflective) material
template <class T> class Metal : public Material<T> {
  public:
    Metal(const Color<T>& a, T f) : albedo(a), fuzz(f < 1 ? f : 1) {}

    virtual bool scatter(const Ray<T>& rIn, const HitRecord<T>& rec,
                         Color<T>& attenuation,
                         Ray<T>& scattered) const override {
        Vec3<T> reflected = reflect(unit(rIn.direction()), rec.normal);
        scattered =
            Ray<T>(rec.p, reflected + fuzz * Vec3<T>::randomInUnitSphere());
        attenuation = albedo;
        return dot(scattered.direction(), rec.normal) > 0;
    }

  private:
    Color<T> albedo;
    T fuzz;
};

} // namespace raytrace

#endif // MATERIAL_H