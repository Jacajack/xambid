cmake_minimum_required(VERSION 3.14)
project(xambid)
find_package(X11 REQUIRED)

set(EXTRA_RELEASE_FLAGS "-O3 -ffast-math -march=native")
set(EXTRA_DEBUG_FLAGS "-fsanitize=address")

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${EXTRA_RELEASE_FLAGS}")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${EXTRA_DEBUG_FLAGS}")

add_subdirectory(xambi)
add_subdirectory(xambid)