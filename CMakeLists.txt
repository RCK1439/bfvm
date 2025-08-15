cmake_minimum_required(VERSION 3.12)
project(bfvm)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(bfvm_VERSION_MAJOR 1)
set(bfvm_VERSION_MINOR 0)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

add_subdirectory(bfc)
add_subdirectory(src)