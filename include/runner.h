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
  void add_to_running_list(if_server* server);
  void del_from_running_list(if_server* server);

  void add_to_running_list(if_client* client);
  void del_from_running_list(if_client* client);

  void run(void);

  // variables
 private:
  std::vector<if_server*> _server_container;
  std::vector<if_client*> _client_container;

  DISALLOW_COPY_AND_ASSIGN(runner);
};
}  // namespace eznetpp


#endif  // INCLUDE_RUNNER_H_

