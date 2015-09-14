#include "udp_echo_server.h"
#include "sys/io_manager.h"

int main(void) {
  eznetpp::sys::io_manager io_mgr(false);
  io_mgr.init(4096);

  udp_echo_server server(&io_mgr);
  server.open(6666);
  
  io_mgr.loop();

  return 0;
}
