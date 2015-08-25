cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
project(eznetpp)

set(PROJECT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

SET(eznetpplib_srcs
#${PROJECT_SOURCE_DIR}/event_dispatcher.cc
#${PROJECT_SOURCE_DIR}/net/tcp/tcp_client.cc
#${PROJECT_SOURCE_DIR}/net/tcp/tcp_server.cc
#${PROJECT_SOURCE_DIR}/net/tcp/tcp_connection.cc
${PROJECT_SOURCE_DIR}/event/event_dispatcher.cc
${PROJECT_SOURCE_DIR}/net/socket.cc
${PROJECT_SOURCE_DIR}/net/tcp/tcp_acceptor.cc
${PROJECT_SOURCE_DIR}/net/tcp/tcp_socket.cc
${PROJECT_SOURCE_DIR}/sys/io_manager.cc
${PROJECT_SOURCE_DIR}/util/logger.cc
)

include_directories(${PROJECT_BINARY_DIR})

add_library(${PROJECT_NAME} STATIC ${eznetpplib_srcs})
