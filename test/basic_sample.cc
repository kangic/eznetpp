#include "../src/runner.h"

// first, I write to example code for making library API concenpt.
// And then I will write to library codes.
int main(void) {
  // it's user class for server
  eznetpp::usr_server s(ip_addr, port_num, max_accept_nums);
  eznetpp::runner r;

  r.add_event_listener(s);

  r.run();

  return 0;
}
