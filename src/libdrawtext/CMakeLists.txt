cmake_minimum_required(VERSION 2.8.9)
project(sigil)

# add source files to the project
add_library(
	drawtext
	STATIC
	drawgl.c
	font.c
	utf8.c
)

# we depend on Freetype---it is available on apt and a cmake find module is available for it
find_package(Freetype REQUIRED)
include_directories(${FREETYPE_INCLUDE_DIRS})
target_link_libraries(drawtext ${FREETYPE_LIBRARIES})
