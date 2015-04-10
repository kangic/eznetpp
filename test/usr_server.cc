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

void usr_server::on_read(eznetpp::connection* conn
    , const std::string& msg, int len, int err_no) {
  printf("received event - on_read()\n");
}

void usr_server::on_write(eznetpp::connection* conn
    , unsigned int len, int err_no) {
  printf("received event - on_write()\n");
}

void usr_server::on_close(eznetpp::connection* conn, int err_no) {
  printf("received event - on_close()\n");
}
