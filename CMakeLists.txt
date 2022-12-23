cmake_minimum_required(VERSION 3.14)
project(xambid)
find_package(X11 REQUIRED)

set(CMAKE_C_RELEASE_FLAGS "${CMAKE_C_RELEASE_FLAGS} -O3 -ffast-math -march=native")

add_subdirectory(xambi)
add_subdirectory(xambid)