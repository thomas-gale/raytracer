#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <iostream>

namespace raytrace {

template <class T> class Vec3 {
  public:
    Vec3() : e{0, 0, 0} {}
    Vec3(T x, T y, T z) : e{x, y, z} {}

    T x() const { return e[0]; }
    T y() const { return e[1]; }
    T z() const { return e[2]; }

    // Primary Functions.

    Vec3 operator-() const { return Vec3(-e[0], -e[1], -e[2]); }
    T operator[](int i) const { return e[i]; }
    T& operator[](int i) { return e[i]; }

    Vec3& operator+=(const Vec3& v) {
        e[0] += v.e[0];
        e[1] += v.e[1];
        e[2] += v.e[2];
        return *this;
    }

    Vec3& operator*=(const T t) {
        e[0] *= t;
        e[1] *= t;
        e[2] *= t;
        return *this;
    }

    Vec3& operator/=(const T t) { return *this *= 1.0 / t; }

    T lengthSquared() const { return e[0] * e[0] + e[1] * e[1] + e[2] * e[2]; }

    T length() const { return std::sqrt(lengthSquared()); }

    // Utility functions

    inline friend std::ostream& operator<<(std::ostream& out, const Vec3& v) {
        return out << v.e[0] << " " << v.e[1] << " " << v.e[2];
    }

    inline friend Vec3 operator+(const Vec3& u, const Vec3& v) {
        return Vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
    }

    inline friend Vec3 operator-(const Vec3& u, const Vec3& v) {
        return Vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
    }

    inline friend Vec3 operator*(const Vec3& u, const Vec3& v) {
        return Vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
    }

    inline friend Vec3 operator*(T t, const Vec3& v) {
        return Vec3(t * v.e[0], t * v.e[1], t * v.e[2]);
    }

    inline friend Vec3 operator*(const Vec3& v, T t) { return t * v; }

    inline friend Vec3 operator/(const Vec3& v, T t) { return (1.0 / t) * v; }

    inline friend T dot(const Vec3& u, const Vec3& v) {
        return u.e[0] * v.e[0] + u.e[1] * v.e[1] + u.e[2] * v.e[2];
    }

    inline friend Vec3 cross(const Vec3& u, const Vec3& v) {
        return Vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
                    u.e[2] * v.e[0] - u.e[0] * v.e[2],
                    u.e[0] * v.e[1] - u.e[1] * v.e[0]);
    }

    inline friend Vec3 unit(Vec3 v) { return v / v.length(); }

    // Stochasitic Utility
    inline static Vec3 random(T min = 0.0, T max = 1.0) {
        return Vec3<T>(randomReal<T>(min, max), randomReal<T>(min, max),
                   randomReal<T>(min, max));
    }

    inline static Vec3 randomInUnitSphere() {
        // Simple random sample in unit cube, reject untill within sphere
        while (true) {
            auto p = Vec3<T>::random(-1, 1);
            if (p.lengthSquared() >= 1)
                continue; // Not in sphere.
            return p;
        }
    }

    // For true Lambertian reflection.
    inline static Vec3 randomUnitVec() {
        return unit(randomInUnitSphere());
    }

  private:
    T e[3];
};

// Useful alises

template <class T> using Point3 = Vec3<T>;
template <class T> using Color = Vec3<T>;

} // namespace raytrace

#endif // VEC3_H
