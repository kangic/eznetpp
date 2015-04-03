#include "../include/runner.h"

#include "./usr_server.h"

// first, I write to example code for making library API concenpt.
// And then I will write to library codes.
int main(void) {
  // it's user class for server
  usr_server s(6666, 5);
  eznetpp::runner r;

  r.register_server(&s);

  r.run();

  return 0;
}
