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

template <class T> HittableList<T> randomScene() {
    using std::make_shared;

    HittableList<T> world;

    auto groundMaterial = make_shared<Lambertian<T>>(Color<T>(0.5, 0.5, 0.5));
    world.add(
        make_shared<Sphere<T>>(Point3<T>(0, -1000, 0), 1000, groundMaterial));

    // Scattered little spheres.
    for (int a = -11; a < 11; ++a) {
        for (int b = -11; b < 11; ++b) {
            auto chooseMat = randomReal<T>();
            Point3<T> center(a + 0.9 * randomReal<T>(), 0.2,
                             b + 0.9 * randomReal<T>());

            if ((center - Point3<T>(4, 0.2, 0)).length() > 0.9) {
                std::shared_ptr<Material<T>> sphereMat;

                if (chooseMat < 0.8) {
                    // Diffuse
                    auto albedo = Color<T>::random() * Color<T>::random();
                    sphereMat = make_shared<Lambertian<T>>(albedo);
                    world.add(make_shared<Sphere<T>>(center, 0.2, sphereMat));
                } else if (chooseMat < 0.95) {
                    // Metal
                    auto albedo = Color<T>::random(0.5, 1);
                    auto fuzz = randomReal<T>(0, 0.5);
                    sphereMat = make_shared<Metal<T>>(albedo, fuzz);
                    world.add(make_shared<Sphere<T>>(center, 0.2, sphereMat));
                } else {
                    // Glass
                    sphereMat = make_shared<Dielectric<T>>(1.5);
                    world.add(make_shared<Sphere<T>>(center, 0.2, sphereMat));
                }
            }
        }
    }

    // Three big lads.
    auto mat1 = make_shared<Dielectric<T>>(1.5);
    world.add(make_shared<Sphere<T>>(Point3<T>(0, 1, 0), 1.0, mat1));

    auto mat2 = make_shared<Lambertian<T>>(Color<T>(0.4, 0.2, 0.1));
    world.add(make_shared<Sphere<T>>(Point3<T>(-4, 1, 0), 1.0, mat2));

    auto mat3 = make_shared<Metal<T>>(Color<T>(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<Sphere<T>>(Point3<T>(4, 1, 0), 1.0, mat3));

    return world;
}

int main() {
    std::cout << "Hello Raytrace" << std::endl;

    // Image.
    const auto aspectRatio = 16.0 / 9.0;
    const int width = 1024;
    const int height = static_cast<int>(width / aspectRatio);
    const int samplesPerPixel = 100;
    const int maxDepth = 50;

    // World.
    auto world = randomScene<real>();

    // Camera.
    Point3<real> lookFrom(13, 2, 3);
    Point3<real> lookAt(0, 0, 0);
    Vec3<real> vUp(0, 1, 0);
    real distToFocus = 10.0;
    real aperture = 0.1;

    Camera<real> cam(lookFrom, lookAt, vUp, 20, aspectRatio, aperture,
                     distToFocus);

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
