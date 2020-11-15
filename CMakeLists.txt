cmake_minimum_required(VERSION 3.10)

# Set the project name
project(Hashlife)

# Add libraries
add_library(Internals hashlife.cpp)
add_library(Loading load.cpp)

# Add the executable
add_executable(Hashlife main.cpp)

# Link
target_link_libraries(Hashlife PUBLIC Internals)
target_link_libraries(Hashlife PUBLIC Loading)
