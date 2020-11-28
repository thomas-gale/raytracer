#include <iostream>
#include <vector>

#include "Common.hpp"

#include "Camera.hpp"
#include "Color.hpp"
#include "HittableList.hpp"
#include "Sphere.hpp"

#include "PixelWindow.hpp"

using namespace raytrace;

template <class T>
Color<T> rayColor(const Ray<T>& r, const Hittable<T>& world, int depth) {
    HitRecord<T> rec;

    // Limit ray bounce
    if (depth <= 0)
        return Color<T>(0, 0, 0);

    if (world.hit(r, 0, infinity, rec)) {
        Point3<T> target = rec.p + rec.normal + Vec3<T>::randomInUnitSphere();
        // Bounce
        return 0.5 *
               rayColor<T>(Ray<T>(rec.p, target - rec.p), world, depth - 1);
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
    const int samplesPerPixel = 16;
    const int maxDepth = 50;

    // World.
    HittableList<double> world;
    world.add(std::make_shared<Sphere<double>>(Point3<double>(0, 0, -1),
                                               0.5)); // Focal sphere.
    world.add(std::make_shared<Sphere<double>>(
        Point3<double>(0, -100.5, -1), 100)); // Massive 'ground' sphere.

    // Camera.
    Camera<double> cam;

    // Render.
    PixelWindow<double> pw(width, height);

    for (int y = height - 1; y >= 0; --y) {
        std::cerr << "\rScanlines remaining: " << y << ' ' << std::flush;
        std::vector<Pixel<double>> line(width);
        for (int x = 0; x < width; ++x) {
            Color<double> pixelColor(0, 0, 0);
            for (int s = 0; s < samplesPerPixel; ++s) {
                auto u = (double(x) + randomReal<double>()) / (width - 1);
                auto v = (double(y) + randomReal<double>()) / (height - 1);
                Ray<double> r = cam.getRay(u, v);
                pixelColor += rayColor(r, world, maxDepth);
            }
            line[x] = Pixel<double>(Point2<int>(x, y), pixelColor);
        }
        pw.setPixels(line, samplesPerPixel);
        pw.draw();
    }

    pw.awaitQuit();
    return 0;
}
