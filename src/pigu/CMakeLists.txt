cmake_minimum_required(VERSION 2.8.9)
project(pigu)
set(CMAKE_BUILD_TYPE Release)

# define compilation flags
set(CMAKE_C_FLAGS "-O3 -march=armv6 -mfpu=vfp -mfloat-abi=hard -Wall -Wextra")

# define our source files
include_directories(include)
set(
	PIGU_SOURCE
	src/graphics
	src/input
	src/pigu
	src/state
	src/time
)

# some include directories we need to look into
include_directories(
	/opt/vc/include
	/opt/vc/include/interface/vcos/pthreads
	/opt/vc/include/interface/vmcs_host/linux
)

# locate some required libraries
find_library(GLES_LIBRARY GLESv2 HINTS /opt/vc/lib)
find_library(EGL_LIBRARY EGL HINTS /opt/vc/lib)
find_library(BCM_LIBRARY bcm_host HINTS /opt/vc/lib)
find_library(VCOS_LIBRARY vcos HINTS /opt/vc/lib)
find_library(RT_LIBRARY rt)

# add the source files to the project
add_library(${PROJECT_NAME} STATIC ${PIGU_SOURCE})

# specify libraries we need to link against
target_link_libraries(
	${PROJECT_NAME}
	${GLES_LIBRARY}
	${EGL_LIBRARY}
	${BCM_LIBRARY}
	${VCOS_LIBRARY}
	${RT_LIBRARY}
)
