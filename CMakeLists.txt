cmake_minimum_required(VERSION 3.10)

# Optimized, but with debug symbols
set(CMAKE_BUILD_TYPE RelWithDebInfo)

# Set the project name
project(Hashlife)

# Add libraries
INCLUDE(FindPkgConfig)
PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
INCLUDE_DIRECTORIES(${SDL2_INCLUDE_DIRS})

add_library(Internals hashlife.cpp)
add_library(Loading load.cpp)

# Add the executable
add_executable(CLI main.cpp)
add_executable(GUI gui.cpp)

# Link
target_link_libraries(CLI ${SDL2_LIBRARIES})
target_link_libraries(CLI Internals)
target_link_libraries(CLI Loading)

target_link_libraries(GUI ${SDL2_LIBRARIES})
target_link_libraries(GUI Internals)
target_link_libraries(GUI Loading)
