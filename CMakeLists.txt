cmake_minimum_required(VERSION 3.10)  # Specify the minimum CMake version
project(RedisProject)                   # Set the project name

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add the executable
add_executable(RedisProject Redis.cpp)

target_link_libraries(RedisProject Ws2_32)