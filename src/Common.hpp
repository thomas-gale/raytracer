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

template <class T> inline T randomDouble() {
    static std::uniform_real_distribution<T> dist(0.0, 1.0);
    static std::mt19937 gen;
    return dist(gen);
}

} // namespace raytrace

// Common Headers
#include "Ray.hpp"
#include "Vec3.hpp"

#endif // COMMON