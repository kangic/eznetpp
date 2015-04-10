// Copyright [2015] <kangic21@gmail.com>

#include "../include/tcp_connection.h"

#include <sys/socket.h>

namespace eznetpp {

const int MAX_SOCK_ID = 0xFFFF;

tcp_connection::tcp_connection() {
}

tcp_connection::~tcp_connection() {
}

int tcp_connection::read(std::string data, int* received_len) {
  return 0;
}

int tcp_connection::write(const std::string& data, int len) {
  return send(sock_id, data.c_str(), len, 0);
}

int tcp_connection::close() {
  int ret = 0;
  return ret;
}

}  // namespace eznetpp
