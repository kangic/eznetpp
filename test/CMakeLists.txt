cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
project(eznetpptest)

set(PROJECT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

SET(eznetpptest_srcs
${PROJECT_SOURCE_DIR}/basic_sample.cc
${PROJECT_SOURCE_DIR}/usr_server.cc
)

include_directories(${PROJECT_BINARY_DIR})
add_executable(basic_sample ${eznetpptest_srcs})
target_link_libraries(basic_sample ${CMAKE_CURRENT_SOURCE_DIR}/../build/libeznetpp.a)
include_directories(${PROJECT_INCLUDE_DIR})

