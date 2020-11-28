#ifndef COLOR_H
#define COLOR_H

#include "Vec3.hpp"

namespace raytrace {

template <class T>
uint32_t convertRGBA(const Color<T>& pixelColor, int samplesPerPixel) {
    auto r = pixelColor.x();
    auto g = pixelColor.y();
    auto b = pixelColor.z();

    // Div color by number of samples & gamma correct for gamma 2.0
    auto scale = 1.0 / samplesPerPixel;
    r = std::sqrt(scale * r);
    g = std::sqrt(scale * g);
    b = std::sqrt(scale * b);

    uint32_t c = static_cast<uint8_t>(255.999 * clamp(r, 0.0, 0.999));
    c <<= 8;
    c |= static_cast<uint8_t>(255.999 * clamp(g, 0.0, 0.999));
    c <<= 8;
    c |= static_cast<uint8_t>(255.999 * clamp(b, 0.0, 0.999));
    c <<= 8;
    return c;
}

} // namespace raytrace
#endif // COLOR_H