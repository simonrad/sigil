cmake_minimum_required(VERSION 2.8.12)
project(sigil)
set(CMAKE_BUILD_TYPE Release)

# add our source files to the project
add_subdirectory(src)
include_directories(include)

# generate the library from our source code
add_library(sigil SHARED ${SOURCE})

# we depend on GLEW---it is available on apt and a cmake 'find' module is available for it
find_package(GLEW REQUIRED)
if(GLEW_FOUND)
	include_directories(${GLEW_INCLUDE_DIRS})
	target_link_libraries(sigil ${GLEW_LIBRARIES})
endif()

# we depend on GLFW3, which is not available on apt---we integrate it per the instructions at http://www.glfw.org/docs/latest/build.html;
# note that this only works on Linux; since there's no easy way to build GLFW3 using MinGW on-the-fly, we just link to pre-compiled libs

if(MINGW)
	# when using MinGW, we link to pre-compiled libs
	#include_directories(src/glfw/include/GLFW)
	#target_link_libraries(sigil /glfw/lib/mingw32/libglfw3.a)
else()
	message(you screwed up)
	# with all other systems we can build straight from GLFW3 source alongside SIGIL
	add_subdirectory(src/glfw)
	include_directories(src/glfw/include)
	target_link_libraries(sigil glfw ${GLFW_LIBRARIES})
endif()

# we depend on Freetype---it is available on apt and a cmake find module is available for it
find_package(Freetype REQUIRED)
if(FREETYPE_FOUND)
	include_directories(${FREETYPE_INCLUDE_DIRS})
	target_link_libraries(sigil ${FREETYPE_LIBRARIES})
endif()

# we depend on OpenAL---it is available on apt and a cmake find module is available for it
find_package(OpenAL REQUIRED)
if(OPENAL_FOUND)
	include_directories(${OPENAL_INCLUDE_DIR})
	target_link_libraries(sigil ${OPENAL_LIBRARY})
endif()

# end of root CMakeLists.txt
