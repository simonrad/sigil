cmake_minimum_required(VERSION 2.8.12)
project(sigil)

# add source files to the project
add_library(
	internal
	STATIC
	circle.c
	line.c
	point.c
	rectangle.c
	shaders.c
	sprite.c
	text.c
	triangle.c
)