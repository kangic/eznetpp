#include "./usr_server.h"

#include <cstdio>

usr_server::usr_server(void) {
}

usr_server::usr_server(int port, int max_accepts) {
}

usr_server::~usr_server(void) {
}

void usr_server::on_accept(const eznetpp::connection& conn, int err_no) {
  printf("received event - on_accpet()\n");
}

void usr_server::on_read(const eznetpp::connection& conn
    , const std::string& msg, int len, int err_no) {
  printf("received event - on_read()\n");
}

void usr_server::on_write(const eznetpp::connection& conn
    , unsigned int len, int err_no) {
  printf("received event - on_write()\n");
}

void usr_server::on_close(const eznetpp::connection& conn, int err_no) {
  printf("received event - on_close()\n");
}
