cmake_minimum_required(VERSION 2.8.12)
project(sigil)

# add source files to the project
add_library(
	util
	STATIC
	images.c
	shader.c
	transform.c
)

# if we're running in Windows, then include the OpenGL debugging features
if(WIN32)
	#set(SOURCE ${SOURCE} ${CMAKE_CURRENT_SOURCE_DIR}/gldebugging.c PARENT_SCOPE)
	#set(HEADERS ${HEADERS} ${CMAKE_CURRENT_SOURCE_DIR}/gldebugging.h PARENT_SCOPE)
endif()
