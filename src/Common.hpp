#ifndef COMMON_H
#define COMMON_H

#include <cmath>
#include <limits>
#include <memory>

namespace raytrace {

// Constants
const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// Utility functions
template <class T> inline T degToRad(T deg) { return deg * pi / 180.0; }

} // namespace raytrace

// Common Headers
#include "Ray.hpp"
#include "Vec3.hpp"

#endif // COMMON