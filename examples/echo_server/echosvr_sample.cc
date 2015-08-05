#include "./echo_server.h"

int main(void) {
  // it's user class for server
  echo_server s;
  s.set_env(6666, 5, 4);

  s.start_async_io();

  return 0;
}
