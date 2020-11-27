#ifndef COLOR_H
#define COLOR_H

#include "Vec3.hpp"

namespace raytrace {

template <class T> uint32_t convertRGBA(Color<T> pixelColor) {
    uint32_t c = static_cast<uint8_t>(255.999 * pixelColor.x());
    c <<= 8;
    c |= static_cast<uint8_t>(255.999 * pixelColor.y());
    c <<= 8;
    c |= static_cast<uint8_t>(255.999 * pixelColor.z());
    c <<= 8;
    return c;
}

} // namespace raytrace
#endif // COLOR_H