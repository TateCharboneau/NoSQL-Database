cmake_minimum_required(VERSION 3.10)  # Specify the minimum CMake version
project(Redis-From-Scratch)            # Set the project name

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add the executable directly with the correct source files
add_executable(Redis-From-Scratch main.cpp redisserver.cpp serversocket.cpp)

# Include the directory containing the header files (if they are in 'src' or other subdirectories)
include_directories(${CMAKE_SOURCE_DIR})  # Adds the current directory (if your headers are in the root)

# Set default build type to Release if not specified
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

# Enable optimization for Release build
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

# Link necessary libraries
target_link_libraries(Redis-From-Scratch Ws2_32)  # Windows sockets library