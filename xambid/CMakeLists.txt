add_executable(xambid
	"${CMAKE_CURRENT_SOURCE_DIR}/src/xambid.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/socket.cpp"
)

target_link_libraries(xambid PRIVATE xambi)

set_target_properties(xambid PROPERTIES 
	CXX_STANDARD_REQUIRED TRUE
	CXX_STANDARD 17
	C_STANDARD_REQUIRED TRUE
	C_STANDARD 99
)