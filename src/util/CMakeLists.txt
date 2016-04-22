cmake_minimum_required(VERSION 2.8.12)
project(sigil)

# add source files to the project
set(
	SOURCE
	${SOURCE}
	${CMAKE_CURRENT_SOURCE_DIR}/images.c
	${CMAKE_CURRENT_SOURCE_DIR}/shader.c
	${CMAKE_CURRENT_SOURCE_DIR}/transform.c
	PARENT_SCOPE
)

# add header files to the project
set(
	HEADERS
	${HEADERS}
	${CMAKE_CURRENT_SOURCE_DIR}/images.h
	${CMAKE_CURRENT_SOURCE_DIR}/shader.h
	${CMAKE_CURRENT_SOURCE_DIR}/transform.h
	PARENT_SCOPE
)

# if we're running in Windows, then include the OpenGL debugging features
if(WIN32)
	set(SOURCE ${SOURCE} ${CMAKE_CURRENT_SOURCE_DIR}/gldebugging.c PARENT_SCOPE)
	set(HEADERS ${HEADERS} ${CMAKE_CURRENT_SOURCE_DIR}/gldebugging.h PARENT_SCOPE)
endif()

