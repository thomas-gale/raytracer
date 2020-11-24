cmake_minimum_required(VERSION 3.17.2)
project(Learn
    VERSION 0.1.0
    DESCRIPTION "Raytracer"
    LANGUAGES CXX)

# Project wide C++ standard
set(CMAKE_CXX_STANDARD 17)

# Add module path in case this is project root
if(PROJECT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/modules/" ${CMAKE_MODULE_PATH})
endif()

# Add libraries (submodules of third party and external libraries)
add_subdirectory(lib)

# Recurse into src.
add_subdirectory(src)