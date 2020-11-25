#ifndef COLOR_H
#define COLOR_H

#include "Vec3.hpp"

namespace raytrace {

template <class T> void writeColor(std::ostream& out, Color<T> pixelColor) {
    out << static_cast<int>(255.999 * pixelColor.x()) << " "
        << static_cast<int>(255.999 * pixelColor.y()) << " "
        << static_cast<int>(255.999 * pixelColor.z()) << "\n";
}

} // namespace raytrace
#endif // COLOR_H