cmake_minimum_required(VERSION 2.8.12)
project(sigil)
set(CMAKE_BUILD_TYPE Release)

# add our source files to the project
include_directories(include)
add_subdirectory(src)

# end of root CMakeLists.txt
