#include "../include/runner.h"

#include "./echo_server.h"

int main(void) {
  // it's user class for server
  echo_server s;
  s.set_env(6666, 5);
  eznetpp::runner r;

  r.add_to_list(&s);

  r.run();

  return 0;
}
