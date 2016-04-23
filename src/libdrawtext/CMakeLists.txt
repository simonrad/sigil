cmake_minimum_required(VERSION 2.8.12)
project(sigil)

# add source files to the project
add_library(
	drawtext
	STATIC
	drawgl.c
	font.c
	utf8.c
)