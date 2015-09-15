cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
project(eznetpp_examples)

set(PROJECT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

# TCP Echo Sample
SET(eznetpp_tcp_echo_server_sample_srcs
${PROJECT_SOURCE_DIR}/tcp_echo/tcp_echosvr_sample.cc
${PROJECT_SOURCE_DIR}/tcp_echo/tcp_echo_server.cc
${PROJECT_SOURCE_DIR}/tcp_echo/tcp_echosvr_session.cc
)

SET(eznetpp_tcp_echo_client_sample_srcs
${PROJECT_SOURCE_DIR}/tcp_echo/tcp_echoclient_sample.cc
${PROJECT_SOURCE_DIR}/tcp_echo/tcp_echo_client.cc
)

# UDP Echo Sample
SET(eznetpp_udp_echo_server_sample_srcs
${PROJECT_SOURCE_DIR}/udp_echo/udp_echosvr_sample.cc
${PROJECT_SOURCE_DIR}/udp_echo/udp_echo_server.cc
)


find_package(Threads)
include_directories(${PROJECT_BINARY_DIR})

add_executable(tcp_echosvr_sample ${eznetpp_tcp_echo_server_sample_srcs})
target_link_libraries(tcp_echosvr_sample ${CMAKE_CURRENT_SOURCE_DIR}/../build/libeznetpp.a)
target_link_libraries(tcp_echosvr_sample ${CMAKE_THREAD_LIBS_INIT})

add_executable(tcp_echoclient_sample ${eznetpp_tcp_echo_client_sample_srcs})
target_link_libraries(tcp_echoclient_sample ${CMAKE_CURRENT_SOURCE_DIR}/../build/libeznetpp.a)
target_link_libraries(tcp_echoclient_sample ${CMAKE_THREAD_LIBS_INIT})

add_executable(udp_echosvr_sample ${eznetpp_udp_echo_server_sample_srcs})
target_link_libraries(udp_echosvr_sample ${CMAKE_CURRENT_SOURCE_DIR}/../build/libeznetpp.a)
target_link_libraries(udp_echosvr_sample ${CMAKE_THREAD_LIBS_INIT})


