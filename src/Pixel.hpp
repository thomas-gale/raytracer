#ifndef PIXEL_H
#define PIXEL_H

#include "Vec2.hpp"
#include "Vec3.hpp"

namespace raytrace {

template <class T> class Pixel {
  public:
    Pixel() {}
    Pixel(Point2<int> loc, Color<T> col) : loc(loc), col(col) {}

    const Point2<int>& location() const { return loc; }
    const Color<T>& color() const { return col; }

  private:
    Point2<int> loc;
    Color<T> col;
};

} // namespace raytrace

#endif // VEC2_H