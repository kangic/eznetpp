// Copyright [2015] <kangic21@gmail.com>

#include "../include/dummy_connection.h"

namespace eznetpp {

const int MAX_SOCK_ID = 0xFFFF;

dummy_connection::dummy_connection()
: sock_id(-1) {
}

dummy_connection::~dummy_connection() {
}

void dummy_connection::socket(int id) {
  sock_id = id;
}

int dummy_connection::socket() {
  return sock_id;
}

}  // namespace eznetpp
