cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
project(eznetpp_examples)

set(PROJECT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

SET(eznetpp_echo_server_sample_srcs
${PROJECT_SOURCE_DIR}/tcp_echo/echosvr_sample.cc
${PROJECT_SOURCE_DIR}/tcp_echo/echo_server.cc
${PROJECT_SOURCE_DIR}/tcp_echo/echosvr_session.cc
)

SET(eznetpp_echo_client_sample_srcs
${PROJECT_SOURCE_DIR}/tcp_echo/echoclient_sample.cc
${PROJECT_SOURCE_DIR}/tcp_echo/echo_client.cc
)

find_package(Threads)
include_directories(${PROJECT_BINARY_DIR})
add_executable(echosvr_sample ${eznetpp_echo_server_sample_srcs})
target_link_libraries(echosvr_sample ${CMAKE_CURRENT_SOURCE_DIR}/../build/libeznetpp.a)
target_link_libraries(echosvr_sample ${CMAKE_THREAD_LIBS_INIT})
add_executable(echoclient_sample ${eznetpp_echo_client_sample_srcs})
target_link_libraries(echoclient_sample ${CMAKE_CURRENT_SOURCE_DIR}/../build/libeznetpp.a)
target_link_libraries(echoclient_sample ${CMAKE_THREAD_LIBS_INIT})

