#include "../include/runner.h"

#include "./echo_client.h"

int main(void) {
  // it's user class for server
  echo_client c;
  c.set_server_info("127.0.0.1", 6666);
  eznetpp::runner r;

  r.add_to_running_list(&c);

  r.run();

  return 0;
}
