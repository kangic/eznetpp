#include "./echo_client.h"

#include "./echoclient_connection.h"

#include <cstdio>

echo_client::echo_client(void) {
}

echo_client::~echo_client(void) {
}

void echo_client::on_connect(int client_id) {
  eznetpp::connection* conn = new echoclient_connection();
  conn->socket_id(client_id);
  set_client_connection(conn);

  printf("received event - on_connect() : socket id %d\n", conn->socket_id());

  std::string hellomsg = "hello eznetpp";
  conn->write_to_socket(hellomsg, hellomsg.length());
}
