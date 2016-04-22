cmake_minimum_required(VERSION 2.8.12)
project(sigil)

# add source files to the project
set(
	SOURCE
	${SOURCE}
	${CMAKE_CURRENT_SOURCE_DIR}/drawgl.c
	${CMAKE_CURRENT_SOURCE_DIR}/font.c
	${CMAKE_CURRENT_SOURCE_DIR}/utf8.c
	PARENT_SCOPE
)

# add header files to the project
set(
	HEADERS
	${HEADERS}
	${CMAKE_CURRENT_SOURCE_DIR}/drawtext.h.h
	${CMAKE_CURRENT_SOURCE_DIR}/drawtext_impl.h
	PARENT_SCOPE
)