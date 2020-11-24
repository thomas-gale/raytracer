add_executable(raytrace
    Main.cpp)

find_package(OpenGL REQUIRED)

target_link_libraries(raytrace
    ${OPENGL_LIBRARIES}
    glfw)
