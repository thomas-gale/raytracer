cmake_minimum_required(VERSION 3.17.2)
project(Learn
    VERSION 0.1.0
    DESCRIPTION "Raytracer"
    LANGUAGES CXX)

# Project wide C++ standard
set(CMAKE_CXX_STANDARD 17)

# Recurse into src.
add_subdirectory(src)
