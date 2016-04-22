cmake_minimum_required(VERSION 2.8.12)
project(sigil)

# add source files to the project
set(
	SOURCE
	${SOURCE}
	${CMAKE_CURRENT_SOURCE_DIR}/circle.c
	${CMAKE_CURRENT_SOURCE_DIR}/line.c
	${CMAKE_CURRENT_SOURCE_DIR}/point.c
	${CMAKE_CURRENT_SOURCE_DIR}/rectangle.c
	${CMAKE_CURRENT_SOURCE_DIR}/shaders.c
	${CMAKE_CURRENT_SOURCE_DIR}/sprite.c
	${CMAKE_CURRENT_SOURCE_DIR}/text.c
	${CMAKE_CURRENT_SOURCE_DIR}/triangle.c
	PARENT_SCOPE
)

# add header files to the project
set(
	HEADERS
	${HEADERS}
	${CMAKE_CURRENT_SOURCE_DIR}/circle.h
	${CMAKE_CURRENT_SOURCE_DIR}/line.h
	${CMAKE_CURRENT_SOURCE_DIR}/point.h
	${CMAKE_CURRENT_SOURCE_DIR}/rectangle.h
	${CMAKE_CURRENT_SOURCE_DIR}/shaders.h
	${CMAKE_CURRENT_SOURCE_DIR}/sprite.h
	${CMAKE_CURRENT_SOURCE_DIR}/text.h
	${CMAKE_CURRENT_SOURCE_DIR}/triangle.h
	PARENT_SCOPE
)