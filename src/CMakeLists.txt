cmake_minimum_required(VERSION 2.8.9)
project(sigil)

# allow static libs to be compiled into shared libs
if(!MINGW)
	set(CMAKE_C_FLAGS "-fPIC")
endif()

# set the installation directory
set(CMAKE_INSTALL_PREFIX "/usr/lib")

# add source files to the project
add_library(sigil SHARED sl.c)

# add subdirectories to the project (only Linux projects add GLFW, and do this one dir level up)
add_subdirectory(util)
add_subdirectory(internal)
add_subdirectory(soil)
add_subdirectory(libdrawtext)

# link the libs we've built in the subdirectories
target_link_libraries(sigil internal drawtext util soil)

# we depend on GLEW---it is available on apt and a cmake 'find' module is available for it
find_package(GLEW REQUIRED)
if(GLEW_FOUND)
	include_directories(${GLEW_INCLUDE_DIRS})
	target_link_libraries(sigil ${GLEW_LIBRARIES})
endif()

# we depend on GLFW3, which is not available on apt---we integrate it per the instructions at http://www.glfw.org/docs/latest/build.html;
# note that this only works on Linux; since there's no easy way to build GLFW3 using MinGW on-the-fly, we just link to pre-compiled libs

# detect if we're on a raspberry pi-like platform
include(CheckIncludeFile)
check_include_file(/opt/vc/include/bcm_host.h BCMHOST)
message("BCM host:" ${BCMHOST})

if(MINGW)				# Windows MinGW compiler
	# when using MinGW, we link to pre-compiled libs instead
	include_directories(src/glfw/include/GLFW)
	target_link_libraries(sigil ${CMAKE_SOURCE_DIR}/glfw/lib/mingw32/glfw3dll.a)
elseif(BCMHOST)			# Raspberry Pi
	message("this is an RPi")
else()					# any other Linux system
	# don't bother building the GLFW docs, examples, or tests
	set(GLFW_BUILD_DOCS OFF CACHE BOOL "Build the GLFW documentation")
	set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "Build the GLFW example programs")
	set(GLFW_BUILD_TESTS OFF CACHE BOOL "Build the GLFW test programs")

	# with all other systems we can build straight from GLFW3 source alongside SIGIL
	add_subdirectory(glfw)
	include_directories(glfw/include)
	target_link_libraries(sigil glfw ${GLFW_LIBRARIES})
endif()

# we depend on OpenAL
if(MINGW)
	# when using MinGW, we link to pre-compiled libs instead
	include_directories(${CMAKE_SOURCE_DIR}/openal/include)
	target_link_libraries(sigil ${CMAKE_SOURCE_DIR}/openal/lib/mingw32/libOpenAL32.dll.a)
else()
	# on other platforms, we can just use whatever we downloaded from apt
	find_package(OpenAL REQUIRED)
	if(OPENAL_FOUND)
		include_directories(${OPENAL_INCLUDE_DIR})
		target_link_libraries(sigil ${OPENAL_LIBRARY})
	endif()
endif()

# we depend on OpenGL---it should already be installed and we just need to link to it
if(WIN32)
	target_link_libraries(sigil opengl32)
else()
	target_link_libraries(sigil GL)
endif()

# configure our make install command
if(WIN32)
else()
	install(TARGETS sigil DESTINATION ${CMAKE_INSTALL_PREFIX})
endif()
