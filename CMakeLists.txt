cmake_minimum_required(VERSION 3.14)
project(xambid)
find_package(X11 REQUIRED)

set(CMAKE_C_RELEASE_FLAGS "${CMAKE_C_RELEASE_FLAGS} -O3 -ffast-math -march=native")

add_executable(xambid
	"${CMAKE_CURRENT_SOURCE_DIR}/src/xambid.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/x11_context.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/capture_x11.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/capture_x11_shm.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/capture_x11_nvfbc.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/socket.cpp"
)

target_include_directories(xambid PRIVATE
	"${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_link_libraries(xambid
	X11::X11
	X11::Xutil
	X11::Xext
)

set_target_properties(xambid PROPERTIES
	CXX_STANDARD_REQUIRED TRUE
	CXX_STANDARD 17
	C_STANDARD_REQUIRED TRUE
	C_STANDARD 99
)
