// Copyright [2015] <kangic21@gmail.com>

#include "../include/dummy_connection.h"

namespace eznetpp {

const int MAX_SOCK_ID = 0xFFFF;

dummy_connection::dummy_connection() {
}

dummy_connection::~dummy_connection() {
}

int dummy_connection::read(std::string data, int* received_len) {
  data = "ok read it!";
  *received_len = data.length();

  return 0;
}

int dummy_connection::write(const std::string& data, int len) {
  return 0;
}

int dummy_connection::close() {
  return 0;
}

}  // namespace eznetpp
