cmake_minimum_required(VERSION 2.8.12)
project(sigil)

# add source files to the project
set(
	SOURCE
	${SOURCE}
	${CMAKE_CURRENT_SOURCE_DIR}/image_DXT.c
	${CMAKE_CURRENT_SOURCE_DIR}/image_helper.c
	${CMAKE_CURRENT_SOURCE_DIR}/SOIL.c
	${CMAKE_CURRENT_SOURCE_DIR}/stb_image_aug.c
	PARENT_SCOPE
)

# add header files to the project
set(
	HEADERS
	${HEADERS}
	${CMAKE_CURRENT_SOURCE_DIR}/image_DXT.h
	${CMAKE_CURRENT_SOURCE_DIR}/image_helper.h
	${CMAKE_CURRENT_SOURCE_DIR}/soil.h
	${CMAKE_CURRENT_SOURCE_DIR}/stb_image_aug.h
	${CMAKE_CURRENT_SOURCE_DIR}/stbi_DDS_aug.h
	${CMAKE_CURRENT_SOURCE_DIR}/stbi_DD_aug_c.h
	PARENT_SCOPE
)