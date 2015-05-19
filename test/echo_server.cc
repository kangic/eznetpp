#include "./echo_server.h"

#include "./echosvr_connection.h"

#include <cstdio>

echo_server::echo_server(void) {
}

echo_server::echo_server(int port, int max_accepts) {
}

echo_server::~echo_server(void) {
}

void echo_server::on_accept(int client_id) {
  eznetpp::connection* conn = new echosvr_connection();
  conn->socket_id(client_id);
  add_to_polling_list(conn);

  printf("received event - on_accpet() : socket id %d\n", conn->socket_id());
}
