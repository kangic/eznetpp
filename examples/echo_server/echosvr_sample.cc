#include <csignal>

#include "echo_server.h"
#include "sys/io_manager.h"

int main(void) {
  eznetpp::sys::io_manager io_mgr(4, false);
  io_mgr.init(4096);

  echo_server server(&io_mgr);
  server.open(6666, 5);
  
  io_mgr.loop();

  return 0;
}
