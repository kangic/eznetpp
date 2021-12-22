cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
project(eznetpp)

set(PROJECT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

SET(eznetpplib_srcs
${PROJECT_SOURCE_DIR}/event/event_dispatcher.cc
${PROJECT_SOURCE_DIR}/net/if_socket.cc
${PROJECT_SOURCE_DIR}/net/tcp/tcp_acceptor.cc
${PROJECT_SOURCE_DIR}/net/tcp/tcp_socket.cc
${PROJECT_SOURCE_DIR}/net/udp/udp_socket.cc
${PROJECT_SOURCE_DIR}/sys/io_manager.cc
${PROJECT_SOURCE_DIR}/util/logger.cc
        event/event_handler.h)

set(EZNETPP_VERSION_MAJOR 0)
set(EZNETPP_VERSION_MINOR 1)
set(EZNETPP_VERSION_PATCH 0)
set(EZNETPP_VERSION
${EZNETPP_VERSION_MAJOR}.${EZNETPP_VERSION_MINOR}.${EZNETPP_VERSION_PATCH}
)

set_target_properties(${TARGET} PROPERTIES 
  VERSION ${BAR_VERSION} 
  SOVERSION ${BAR_VERSION_MAJOR}
)


include_directories(${PROJECT_BINARY_DIR})

add_library(${PROJECT_NAME} STATIC ${eznetpplib_srcs})
