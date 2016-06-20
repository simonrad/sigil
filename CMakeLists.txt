# CMakeLists.txt file for building SIGIL
# Currently supported compilers/platforms:
# MinGW32; MSVC (32- and 64-bit) 10, 11, 12, 14; Linux GCC, Raspberry Pi
# Geoff Nagy

# not the most current version, but we want older CMake systems to be able to build this, too
cmake_minimum_required(VERSION 2.8.9)
project(sigil)
set(CMAKE_BUILD_TYPE Release)

# SIGIL versioning
set(SIGIL_VERSION_MAJOR 0)
set(SIGIL_VERSION_MINOR 8)
set(SIGIL_VERSION_REVISION 9)

# defaults for libraries we use
set(USE_GLFW OFF)
set(USE_GLES OFF)

# print our version number
message("------------------------------")
message("   This is SIGIL ${SIGIL_VERSION_MAJOR}.${SIGIL_VERSION_MINOR}.${SIGIL_VERSION_REVISION}")
message("------------------------------")

#--------------------------------------------------------------------
# put together the sources files that we'll need
#--------------------------------------------------------------------

# define our source files
include_directories(include)
set(
	SIGIL_SOURCE
	src/sl
	src/internal/circle
	src/internal/line
	src/internal/point
	src/internal/rectangle
	src/internal/sound
	src/internal/sprite
	src/internal/text
	src/internal/triangle
	src/libdrawtext/drawgl			# from the handy libdrawtext library: http://nuclear.mutantstargoat.com/sw/libdrawtext/
	src/libdrawtext/font
	src/libdrawtext/utf8
	src/soil/image_DXT				# from the handy SOIL library: http://www.lonesock.net/soil.html
	src/soil/image_helper
	src/soil/SOIL
	src/soil/stb_image_aug
	src/util/images
	src/util/shader
	src/util/transform
)

# set the installation directory; this only used in linux-based systems
set(CMAKE_INSTALL_PREFIX "/usr/lib")

# enable all compiler warnings, except in MSVC, since it own headers annoyingly generate around 4000 warnings
if(NOT MSVC)
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
else()
	# we're all big kids here, so disable those annoying "considered unsafe" warnings in MSVC
	add_definitions(-D_CRT_SECURE_NO_DEPRECATE)
endif()

#--------------------------------------------------------------------
# detect Raspberry Pi platform
#--------------------------------------------------------------------

# the presence of bcm_host.h means we're probably running on an RPi; this might be a sketchy-enough test as of right now that we should print it out for debugging purposes
find_file(FOUND_BCM bcm_host.h /opt/vc/include)
if(FOUND_BCM)
	message(STATUS "Detected a Raspberry Pi platform (bcm_host.h was found)")
else()
	message(STATUS "Did NOT detect a Raspberry Pi platform (bcm_host.h not found)")
endif()

#--------------------------------------------------------------------
# include either GLFW- or PIGU-based functionality in our project
#--------------------------------------------------------------------

if(FOUND_BCM)

	# use the PIGU- and GLES2-based version of SIGIL
	set(SIGIL_SOURCE ${SIGIL_SOURCE} src/internal/window_pigu)
	set(SIGIL_SOURCE ${SIGIL_SOURCE} src/internal/shaders_gles)

	# tell config.h to use PIGU, not GLFW
	set(USE_GLFW OFF)

	# tell config.h that we need GLES
	set(USE_GLES ON)

else()

	# use the GLFW- and GL-based version of SIGIL
	set(SIGIL_SOURCE ${SIGIL_SOURCE} src/internal/window_glfw)
	set(SIGIL_SOURCE ${SIGIL_SOURCE} src/internal/shaders_gl)

	# tell config.h to use GLFW, not PIGU
	set(USE_GLFW ON)

	# tell config.h that we don't need GLES
	set(USE_GLES OFF)

endif()

# now that we have all the SIGIL source files, add them to the SIGIL shared lib
add_library(${PROJECT_NAME} SHARED ${SIGIL_SOURCE})

#--------------------------------------------------------------------
# locate underlying context/window/input library
#--------------------------------------------------------------------

# everything uses GLFW right now (except for the Raspberry Pi)
if(USE_GLFW)

	# turn off unnecessary GLFW components
	set(GLFW_BUILD_DOCS OFF CACHE BOOL "Build the GLFW documentation")
	set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "Build the GLFW example programs")
	set(GLFW_BUILD_TESTS OFF CACHE BOOL "Build the GLFW test programs")

	# build GLFW from source
	add_subdirectory(${CMAKE_SOURCE_DIR}/deps/glfw)
	include_directories(${CMAKE_SOURCE_DIR}/deps/glfw/include)
	target_link_libraries(${PROJECT_NAME} glfw)

else()

	# use the cmake file we added to PIGU
	add_subdirectory(${CMAKE_SOURCE_DIR}/deps/pigu)
	include_directories(${CMAKE_SOURCE_DIR}/deps/pigu/include)
	target_link_libraries(${PROJECT_NAME} pigu ${PIGU_LIBRARIES})

endif()

#--------------------------------------------------------------------
# OpenGL extension handling for non-RPi systems
#--------------------------------------------------------------------

# RPi's use GLES; everything else uses GLEW
if(USE_GLES)

	# add in the GLES 2.0 libraries and other dependencies
	include_directories(/opt/vc/include)
	find_library(GLES_LIBS NAMES GLESv2 EGL bcm_host vcos rt PATHS /opt/vc/lib)
	target_link_libraries(${PROJECT_NAME} ${GLES_LIBS})

else()

	# add in the GLEW libraries
	find_package(GLEW REQUIRED)
	include_directories(${GLEW_INCLUDE_DIRS})
	target_link_libraries(${PROJECT_NAME} ${GLEW_LIBRARIES})

endif()

#--------------------------------------------------------------------
# find our font library, freetype
#--------------------------------------------------------------------

# compile freetype alongside our windows builds
if(WIN32)

	# add in the freetype CMakeLists.txt file to our CMake project
	add_subdirectory(${CMAKE_SOURCE_DIR}/deps/freetype2)
	include_directories(${CMAKE_SOURCE_DIR}/deps/freetype2/include)
	include_directories(${CMAKE_SOURCE_DIR}/deps/freetype2/include/freetype)
	target_link_libraries(${PROJECT_NAME} freetype ${FREETYPE_LIBRARIES})

# Raspberry Pi and Linux can just use an already-installed version
else()

	# give the RPi a little hint regarding the whereabouts of freetype
	if(FOUND_BCM)
		set(FREETYPE_INCLUDE_DIR_freetype2 "/usr/local/include")
		set(FREETYPE_INCLUDE_DIR_ft2build "/usr/local/include/freetype2")
		set(FREETYPE_INCLUDE_DIRS ${FREETYPE_INCLUDE_DIR_freetype2} ${FREETYPE_INCLUDE_DIR_ft2build})
	endif()

	# freetype is available on apt and a cmake find module is available for it
	find_package(Freetype REQUIRED)
	include_directories(${FREETYPE_INCLUDE_DIRS})
	target_link_libraries(${PROJECT_NAME} ${FREETYPE_LIBRARIES})

endif()

#--------------------------------------------------------------------
# everything uses OpenAL for audio handling
#--------------------------------------------------------------------

# use the OpenAL find_package module to locate OpenAL for us
find_package(OpenAL REQUIRED)
include_directories(${OPENAL_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} ${OPENAL_LIBRARY})

#--------------------------------------------------------------------
# link to the appropriate GL library
#--------------------------------------------------------------------

# we depend on OpenGL---it should already be installed and we just need to link to it
if(WIN32)
	target_link_libraries(${PROJECT_NAME} opengl32)
else()
	target_link_libraries(${PROJECT_NAME} GL)
endif()

#--------------------------------------------------------------------
# setup our config.h file for our conditional compilation
#--------------------------------------------------------------------

# setup our #define preprocessor directives that specify how SIGIL should be compiled
configure_file(${CMAKE_SOURCE_DIR}/src/config.h.in ${CMAKE_BINARY_DIR}/src/config.h)
include_directories(${CMAKE_BINARY_DIR}/src)

#--------------------------------------------------------------------
# installation configuration
#--------------------------------------------------------------------

# configure our make install command
if(WIN32)
	# nothing to install; at this time, the users copies things manually
else()
	install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX})
endif()

# end of root CMakeLists.txt
