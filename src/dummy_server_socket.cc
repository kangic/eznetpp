#include <cstdio>

#include "./dummy_server_socket.h"

namespace eznetpp {

dummy_server_socket::dummy_server_socket(void) {
}

dummy_server_socket::~dummy_server_socket(void) {
}

void dummy_server_socket::handle_read() {
  printf("received event - handle_read()\n");
}

void dummy_server_socket::handle_write() {
  printf("received event - handle_write()\n");
}

}  // namespace eznetpp
