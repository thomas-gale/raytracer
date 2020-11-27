#include <iostream>
#include <vector>

#include <SDL.h>

#include "Color.hpp"
#include "HittableList.hpp"
#include "PixelWindow.hpp"
#include "Ray.hpp"
#include "Sphere.hpp"
#include "Vec3.hpp"

using namespace raytrace;

// Using the quadradic formula
// Performed some simplifications (b = 2h)
template <class T>
T hitSphere(const Point3<T>& center, T radius, const Ray<T>& r) {
    Vec3<T> oc = r.origin() - center;
    auto a = r.direction().lengthSquared();
    auto halfB = dot(oc, r.direction());
    auto c = oc.lengthSquared() - radius * radius;
    auto discriminant = halfB * halfB - a * c;
    if (discriminant < 0) {
        return -1.0;
    }
    return (-halfB - std::sqrt(discriminant)) / a;
}

template <class T> Color<T> rayColor(const Ray<T>& r) {
    auto t = hitSphere(Point3<T>(0, 0, -1), 0.5, r);
    if (t > 0) {
        Vec3<T> n = unit(r.at(t) - Vec3<T>(0, 0, -1));
        return 0.5 * Color<T>(n.x() + 1, n.y() + 1, n.z() + 1);
    }
    Vec3<T> unitDirection = unit(r.direction());
    t = 0.5 * (unitDirection.y() + 1.0);
    return (1.0 - t) * Color<T>(1, 1, 1) + t * Color<T>(0.5, 0.7, 1.0);
}

int main() {
    std::cout << "Hello Raytrace" << std::endl;

    // Image.
    const auto aspectRatio = 16.0 / 9.0;
    const int width = 512;
    const int height = static_cast<int>(width / aspectRatio);

    // Camera.
    auto viewportHeight = 2.0;
    auto viewportWidth = aspectRatio * viewportHeight;
    auto focalLength = 1.0;

    auto origin = Point3<double>(0.0, 0.0, 0.0);
    auto horizontal = Vec3<double>(viewportWidth, 0.0, 0.0);
    auto vertical = Vec3<double>(0.0, viewportHeight, 0.0);
    auto lowerLeftCorner = origin - horizontal / 2 - vertical / 2 -
                           Vec3<double>(0.0, 0.0, focalLength);

    // Render.
    PixelWindow<double> pw(width, height);

    for (int y = height - 1; y >= 0; --y) {
        std::cerr << "\rScanlines remaining: " << y << ' ' << std::flush;
        std::vector<Pixel<double>> line(width);
        for (int x = 0; x < width; ++x) {
            auto u = double(x) / (width - 1);
            auto v = double(y) / (height - 1);
            Ray<double> r(origin, lowerLeftCorner + u * horizontal +
                                      v * vertical - origin);
            Color<double> pixelColor = rayColor(r);
            line[x] = Pixel<double>(Point2<int>(x, y), pixelColor);
        }
        pw.setPixels(line);
        pw.draw();
    }

    pw.awaitQuit();
    return 0;
}
