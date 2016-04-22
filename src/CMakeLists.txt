cmake_minimum_required(VERSION 2.8.12)
project(sigil)

# add subdirectories to the project (glfw has already been added, so we omit that)
add_subdirectory(internal)
add_subdirectory(libdrawtext)
add_subdirectory(soil)
add_subdirectory(util)

# add source files to the project
set(
	SOURCE
	${SOURCE}
	${CMAKE_CURRENT_SOURCE_DIR}/sl.c
	PARENT_SCOPE
)