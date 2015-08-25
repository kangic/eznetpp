#include "echo_server.h"
#include "sys/io_manager.h"

int main(void) {
  eznetpp::sys::io_manager io_mgr;
  io_mgr.init();

  echo_server server(&io_mgr);
  server.open(56789, 5);
  
  io_mgr.loop();

  return 0;
}
