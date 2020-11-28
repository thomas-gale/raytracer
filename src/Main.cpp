#include <chrono>
#include <iostream>
#include <vector>

#include "Common.hpp"

#include "Camera.hpp"
#include "Color.hpp"
#include "HittableList.hpp"
#include "Material.hpp"
#include "Sphere.hpp"

#include "PixelWindow.hpp"

using namespace raytrace;

template <class T>
Color<T> rayColor(const Ray<T>& r, const Hittable<T>& world, int depth) {
    HitRecord<T> rec;

    // Limit ray bounce
    if (depth <= 0)
        return Color<T>(0, 0, 0);

    if (world.hit(r, 0.001, infinity, rec)) {
        Ray<T> scattered;
        Color<T> attenuation;
        if (rec.mat->scatter(r, rec, attenuation, scattered)) {
            return attenuation * rayColor<T>(scattered, world, depth - 1);
        }
        return Color<T>(0, 0, 0);
    }

    // Environment coloring.
    Vec3<T> unitDirection = unit(r.direction());
    auto t = 0.5 * (unitDirection.y() + 1.0);
    return (1.0 - t) * Color<T>(1, 1, 1) + t * Color<T>(0.5, 0.7, 1.0);
}

int main() {
    std::cout << "Hello Raytrace" << std::endl;

    // Image.
    const auto aspectRatio = 16.0 / 9.0;
    const int width = 400;
    const int height = static_cast<int>(width / aspectRatio);
    const int samplesPerPixel = 100;
    const int maxDepth = 50;

    // World.
    auto r = std::cos(pi / 4);
    HittableList<real> world;

    /*auto matGround =
        std::make_shared<Lambertian<real>>(Color<real>(0.8, 0.8, 0));
    auto matCenter =
        std::make_shared<Lambertian<real>>(Color<real>(0.1, 0.2, 0.5));*/
    auto matLeft = std::make_shared<Lambertian<real>>(Color<real>(0, 0, 1));
    auto matRight = std::make_shared<Lambertian<real>>(Color<real>(1, 0, 0));

    /*world.add(
        std::make_shared<Sphere<real>>(Point3<real>(0, -100.5, -1), 100,
                                       matGround)); // Massive 'ground' sphere.
    world.add(
        std::make_shared<Sphere<real>>(Point3<real>(0, 0, -1), 0.5,
    matCenter));*/

    world.add(
        std::make_shared<Sphere<real>>(Point3<real>(-r, 0, -1), r, matLeft));
    world.add(
        std::make_shared<Sphere<real>>(Point3<real>(r, 0, -1), r, matRight));

    // Camera.
    Camera<real> cam(90, aspectRatio);

    // Render (with timer)
    PixelWindow<real> pw(width, height);
    auto start = std::chrono::high_resolution_clock::now();

    for (int y = height - 1; y >= 0; --y) {
        std::cerr << "\rScanlines remaining: " << y << ' ' << std::flush;
        std::vector<Pixel<real>> line(width);
        for (int x = 0; x < width; ++x) {
            Color<real> pixelColor(0, 0, 0);
            for (int s = 0; s < samplesPerPixel; ++s) {
                auto u = (real(x) + randomReal<real>()) / (width - 1);
                auto v = (real(y) + randomReal<real>()) / (height - 1);
                Ray<real> r = cam.getRay(u, v);
                pixelColor += rayColor(r, world, maxDepth);
            }
            line[x] = Pixel<real>(Point2<int>(x, y), pixelColor);
        }
        pw.setPixels(line, samplesPerPixel);
        pw.draw();
    }

    // Display timing info.
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration =
        std::chrono::duration_cast<std::chrono::seconds>(stop - start);
    std::cerr << "\nCompleted: " << duration.count() << "s\n" << std::flush;

    pw.awaitQuit();
    return 0;
}
