// Copyright[2015] <kangic21@gmail.com>

#ifndef INCLUDE_RUNNER_H_
#define INCLUDE_RUNNER_H_

#include <eznetpp.h>

namespace eznetpp {

class if_server;
class if_client;

class runner {
 public:
  runner(void);
  virtual ~runner(void);

 public:
  void register_server(if_server* server);
  void deregister_server(if_server* server);

  void register_client(if_client* client);
  void deregister_client(if_client* client);

  void run(void);

  // variables
 private:
  std::vector<if_server*> _server_container;
  std::vector<if_client*> _client_container;

  DISALLOW_COPY_AND_ASSIGN(runner);
};
}  // namespace eznetpp


#endif  // INCLUDE_RUNNER_H_

