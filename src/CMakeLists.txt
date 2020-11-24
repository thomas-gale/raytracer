add_executable(raytrace
    Main.cpp)

find_package(Corrade REQUIRED 
    Main)

find_package(Magnum REQUIRED 
    GL
    MeshTools
    Primitives
    Shaders
    Sdl2Application)

target_link_libraries(raytrace
    Corrade::Main
    Magnum::Application
    Magnum::GL
    Magnum::Magnum
    Magnum::MeshTools
    Magnum::Primitives
    Magnum::Shaders)
