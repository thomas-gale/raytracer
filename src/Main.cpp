#include <iostream>

#include <SDL.h>

#include "Color.hpp"
#include "PixelWindow.hpp"
#include "Ray.hpp"
#include "Vec3.hpp"

using namespace raytrace;

int main() {
    std::cout << "Hello Raytrace" << std::endl;

    // Settings.
    int width = 640;
    int height = 480;

    PixelWindow<double> pw(width, height);
    pw.setPrettyColor();
    pw.draw();

    pw.setSolidColor(Color<double>(1.0, 1.0, 1.0));
    pw.draw();

    pw.setPixel(100, 100, Color<double>(0.0, 0.0, 0.0));
    pw.draw();

    return 0;
}
