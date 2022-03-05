cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
project(eznetpp_build)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -Wall -O0 -g -std=c++11")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O3 -DNDEBUG -std=c++11")
set(CMAKE_EXPORT_COMPILE_COMMANDS on)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

set(PROJECT_INCLUDE_DIRS 
${CMAKE_CURRENT_SOURCE_DIR}/core
)

include_directories(${PROJECT_INCLUDE_DIRS})
include_directories(${PROJECT_SOURCE_DIR})

add_subdirectory(core)
add_subdirectory(examples)

