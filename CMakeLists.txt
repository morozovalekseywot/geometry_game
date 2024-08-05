cmake_minimum_required(VERSION 3.0)

project(game)

find_package(X11 REQUIRED)
set(CMAKE_CONFIGURATION_TYPES "Debug" "Release")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
file(GLOB SRC *.cpp)
add_executable(game ${SRC})
target_link_libraries(game m X11)
