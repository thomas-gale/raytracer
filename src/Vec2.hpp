#ifndef VEC2_H
#define VEC2_H

namespace raytrace {

template <class T> class Vec2 {
  public:
    Vec2() : e{0, 0} {}
    Vec2(T x, T y) : e{x, y} {}

    T x() const { return e[0]; }
    T y() const { return e[1]; }

  private:
    T e[2];
};

template <class T> using Point2 = Vec2<T>;

} // namespace raytrace

#endif // VEC2_H