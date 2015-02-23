cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
project(eznetpp)

set(PROJECT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

SET(eznetpplib_srcs
${PROJECT_SOURCE_DIR}/runner.cc
${PROJECT_SOURCE_DIR}/event_dispatcher.cc
${PROJECT_SOURCE_DIR}/dummy_server.cc
)

include_directories(${PROJECT_BINARY_DIR})
include_directories(${PROJECT_INCLUDE_DIR})

find_package(Threads)
add_library(${PROJECT_NAME} STATIC ${eznetpplib_srcs})
target_link_libraries(${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT})
