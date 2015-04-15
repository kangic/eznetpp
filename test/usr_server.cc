#include "./usr_server.h"

#include <cstdio>

usr_server::usr_server(void) {
}

usr_server::usr_server(int port, int max_accepts) {
}

usr_server::~usr_server(void) {
}

void usr_server::on_accept(eznetpp::connection* conn, int err_no) {
  printf("received event - on_accpet()\n");
  conn->write("hello", 5);
}
