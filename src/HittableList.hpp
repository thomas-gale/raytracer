#ifndef HITTABLELIST_H
#define HITTABLELIST_H

#include <memory>
#include <vector>

#include "Ray.hpp"
#include "Hittable.hpp"

namespace raytrace {

// Store a list of hittables.
template <class T> class HittableList : public Hittable<T> {
  public:
    HittableList() {}
    HittableList(std::shared_ptr<Hittable<T>> object) { add(object); }

    void clear() { objects.clear(); }
    void add(std::shared_ptr<Hittable<T>> object) { objects.push_back(object); }

    virtual bool hit(const Ray<T>& r, T tMin, T tMax,
                     HitRecord<T>& rec) const override {
        HitRecord<T> tempRec;
        bool hitAnything = false;
        auto closestSoFar = tMax;

        for (const auto& object : objects) {
            if (object->hit(r, tMin, closestSoFar, tempRec)) {
                hitAnything = true;
                closestSoFar = tempRec.t;
                rec = tempRec;
            }
        }

        return hitAnything;
    }

  private:
    std::vector<std::shared_ptr<Hittable<T>>> objects;
};

} // namespace raytrace

#endif // HITTABLELIST_H