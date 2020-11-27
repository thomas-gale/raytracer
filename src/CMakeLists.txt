find_package(SDL2 REQUIRED)

add_executable(raytrace
    Vec2.hpp
    Vec3.hpp
    Color.hpp
    Ray.hpp
    Hittable.hpp
    Sphere.hpp
    Pixel.hpp
    PixelWindow.hpp
    Main.cpp)

target_include_directories(raytrace PUBLIC
    ${SDL2_INCLUDE_DIRS})

target_link_libraries(raytrace PUBLIC
    ${SDL2_LIBRARIES})
