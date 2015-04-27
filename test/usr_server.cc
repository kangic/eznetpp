#include "./usr_server.h"

#include "./usr_connection.h"

#include <cstdio>

usr_server::usr_server(void) {
}

usr_server::usr_server(int port, int max_accepts) {
}

usr_server::~usr_server(void) {
}

void usr_server::on_accept(int client_id) {
  eznetpp::connection* conn = new usr_connection();
  conn->socket_id(client_id);
  add_to_polling_list(conn);

  printf("received event - on_accpet() : socket id %d\n", conn->socket_id());
}
