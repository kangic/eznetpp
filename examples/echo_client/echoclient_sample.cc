#include "./echo_client.h"
#include "sys/io_manager.h"

int main(void) {
  eznetpp::sys::io_manager io_mgr(1, true);
  io_mgr.init();

  echo_client client(&io_mgr);
  client.start();
  
  io_mgr.loop();

  return 0;
}
