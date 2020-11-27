#include <iostream>
#include <vector>

#include "Common.hpp"

#include "Color.hpp"
#include "HittableList.hpp"
#include "Sphere.hpp"

#include "PixelWindow.hpp"

using namespace raytrace;

template <class T>
Color<T> rayColor(const Ray<T>& r, const Hittable<T>& world) {
    HitRecord<T> rec;
    if (world.hit(r, 0, infinity, rec)) {
        return 0.5 * (rec.normal + Color<T>(1, 1, 1));
    }
    Vec3<T> unitDirection = unit(r.direction());
    auto t = 0.5 * (unitDirection.y() + 1.0);
    return (1.0 - t) * Color<T>(1, 1, 1) + t * Color<T>(0.5, 0.7, 1.0);
}

int main() {
    std::cout << "Hello Raytrace" << std::endl;

    // Image.
    const auto aspectRatio = 16.0 / 9.0;
    const int width = 512;
    const int height = static_cast<int>(width / aspectRatio);

    // World.
    HittableList<double> world;
    world.add(std::make_shared<Sphere<double>>(Point3<double>(0, 0, -1),
                                               0.5)); // Focal sphere.
    world.add(std::make_shared<Sphere<double>>(
        Point3<double>(0, -100.5, -1), 100)); // Massive 'ground' sphere.

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
            Ray<double> r(origin,
                          lowerLeftCorner + u * horizontal + v * vertical);
            Color<double> pixelColor = rayColor(r, world);
            line[x] = Pixel<double>(Point2<int>(x, y), pixelColor);
        }
        pw.setPixels(line);
        pw.draw();
    }

    pw.awaitQuit();
    return 0;
}
