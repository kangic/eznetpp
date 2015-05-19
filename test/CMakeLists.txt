cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
project(eznetpptest)

set(PROJECT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

SET(eznetpptest_srcs
${PROJECT_SOURCE_DIR}/echosvr_sample.cc
${PROJECT_SOURCE_DIR}/echo_server.cc
${PROJECT_SOURCE_DIR}/echosvr_connection.cc
)

find_package(Threads)
include_directories(${PROJECT_BINARY_DIR})
add_executable(echosvr_sample ${eznetpptest_srcs})
target_link_libraries(echosvr_sample ${CMAKE_CURRENT_SOURCE_DIR}/../build/libeznetpp.a)
target_link_libraries(echosvr_sample ${CMAKE_THREAD_LIBS_INIT})
include_directories(${PROJECT_INCLUDE_DIR})

