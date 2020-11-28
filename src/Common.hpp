#ifndef COMMON_H
#define COMMON_H

#include <cmath>
#include <limits>
#include <memory>
#include <random>

namespace raytrace {

// Constants
const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// Utility functions
template <class T> inline T degToRad(T deg) { return deg * pi / 180.0; }

template <class T> inline T randomReal(T min = 0.0, T max = 1.0) {
    static std::uniform_real_distribution<T> dist(min, max);
    static std::mt19937 gen;
    return dist(gen);
}

template <class T> inline T clamp(T x, T min, T max) {
    if (x < min)
        return min;
    if (x > max)
        return max;
    return x;
}

} // namespace raytrace

// Common Headers
#include "Ray.hpp"
#include "Vec3.hpp"

#endif // COMMON