cmake_minimum_required(VERSION 2.8.12)
project(sigil)

# add source files to the project
add_library(
	soil
	STATIC
	image_DXT.c
	image_helper.c
	SOIL.c
	stb_image_aug.c
)
