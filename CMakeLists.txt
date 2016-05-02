cmake_minimum_required(VERSION 2.8.9)
project(sigil)
set(CMAKE_BUILD_TYPE Release)

# by default, we don't use GLFW or PIGU
set(USE_PIGU OFF)
set(USE_GLFW OFF)

# define our source files
include_directories(include)
set(
	SIGIL_SOURCE
	src/sl
	src/internal/circle
	src/internal/line
	src/internal/point
	src/internal/rectangle
	src/internal/shaders
	src/internal/sound
	src/internal/sprite
	src/internal/text
	src/internal/triangle
	src/libdrawtext/drawgl
	src/libdrawtext/font
	src/libdrawtext/utf8
	src/soil/image_DXT
	src/soil/image_helper
	src/soil/SOIL
	src/soil/stb_image_aug
	src/util/images
	src/util/shader
	src/util/transform
)

# add source files to the project
add_library(${PROJECT_NAME} SHARED ${SIGIL_SOURCE})

# set the installation directory
set(CMAKE_INSTALL_PREFIX "/usr/lib")

# the presence of bcm_host.h means we're probably running on an RPi; this is a sketchy-enough test as of right now that we should print it out for debugging purposes
find_file(FOUND_BCM bcm_host.h /opt/vc/include)
if(FOUND_BCM)
	message("-- Detected a Raspberry Pi platform (bcm_host.h was found)")
else()
	message("-- Did NOT detect a Raspberry Pi platform (bcm_host.h not found)")
endif()

# RPi's don't use GLEW, but everything else does
if(NOT FOUND_BCM)
	
	# we depend on GLEW---it's on apt and a cmake 'find' module is available for it
	find_package(GLEW REQUIRED)
	if(GLEW_FOUND)

		include_directories(${GLEW_INCLUDE_DIRS})
		target_link_libraries(${PROJECT_NAME} ${GLEW_LIBRARIES})

	endif()

endif()

# the RPi uses the PIGU library for window, input, and context handling
if(FOUND_BCM)

	# use the cmake file we added to PIGU
	add_subdirectory(${CMAKE_SOURCE_DIR}/src/pigu)
	include_directories(${CMAKE_SOURCE_DIR}/src/pigu/include)
	target_link_libraries(${PROJECT_NAME} pigu ${PIGU_LIBRARIES})

	# tell config.h to use PIGU, not GLFW
	set(USE_PIGU ON)

else()

	# when using MinGW on Windows, we link to pre-compiled libs
	if(MINGW)

		include_directories(src/glfw/include/GLFW)
		target_link_libraries(${PROJECT_NAME} ${CMAKE_SOURCE_DIR}/glfw/lib/mingw32/glfw3dll.a)

	else()

		# linux systems can build GLFW from source more easily, but don't bother building docs, examples, or tests
		set(GLFW_BUILD_DOCS OFF CACHE BOOL "Build the GLFW documentation")
		set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "Build the GLFW example programs")
		set(GLFW_BUILD_TESTS OFF CACHE BOOL "Build the GLFW test programs")

		# with all other systems we can build straight from GLFW3 source alongside SIGIL
		add_subdirectory(${CMAKE_SOURCE_DIR}/src/glfw)
		include_directories(${CMAKE_SOURCE_DIR}/src/glfw/include)
		target_link_libraries(${PROJECT_NAME} glfw ${GLFW_LIBRARIES})

	endif()

	# tell config.h to use GLFW, not PIGU
	set(USE_GLFW ON)

endif()

# setup any compile-time variables
configure_file(${CMAKE_SOURCE_DIR}/src/config.h.in ${CMAKE_BINARY_DIR}/src/config.h)
include_directories(${CMAKE_BINARY_DIR}/src)

# give the RPi a little hint with the whereabouts of freetype
if(FOUND_BCM)
	set(FREETYPE_INCLUDE_DIR_freetype2 "/usr/local/include")
	set(FREETYPE_INCLUDE_DIR_ft2build "/usr/local/include/freetype2")
	set(FREETYPE_INCLUDE_DIRS ${FREETYPE_INCLUDE_DIR_freetype2} ${FREETYPE_INCLUDE_DIR_ft2build})
endif()

# we depend on Freetype---it is available on apt and a cmake find module is available for it
find_package(Freetype REQUIRED)
include_directories(${FREETYPE_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${FREETYPE_LIBRARIES})

# we depend on OpenAL
if(MINGW)

	# when using MinGW, we link to pre-compiled libs instead
	include_directories(${CMAKE_SOURCE_DIR}/openal/include)
	target_link_libraries(${PROJECT_NAME} ${CMAKE_SOURCE_DIR}/openal/lib/mingw32/libOpenAL32.dll.a)

else()

	# on other platforms, we can just use whatever we downloaded from apt
	find_package(OpenAL REQUIRED)
	if(OPENAL_FOUND)

		include_directories(${OPENAL_INCLUDE_DIR})
		target_link_libraries(${PROJECT_NAME} ${OPENAL_LIBRARY})

	endif()

endif()

# we depend on OpenGL---it should already be installed and we just need to link to it
if(WIN32)
	target_link_libraries(${PROJECT_NAME} opengl32)
else()
	target_link_libraries(${PROJECT_NAME} GL)
endif()

# configure our make install command
if(WIN32)
else()
	install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX})
endif()

# end of root CMakeLists.txt

