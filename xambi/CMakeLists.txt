add_library(xambi SHARED
	"${CMAKE_CURRENT_SOURCE_DIR}/src/x11_context.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/x11_image.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/capture_backend.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/capture_x11.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/capture_x11_shm.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/capture_x11_nvfbc.cpp"
)

target_include_directories(xambi PUBLIC
	"${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_compile_definitions(xambi PRIVATE
	"XAMBI_FAST_X11_IMAGE_GETPIXEL"
)

target_link_libraries(xambi PUBLIC
	X11::X11
	X11::Xutil
	X11::Xext
)

set_target_properties(xambi PROPERTIES 
	CXX_STANDARD_REQUIRED TRUE
	CXX_STANDARD 17
	C_STANDARD_REQUIRED TRUE
	C_STANDARD 99
)