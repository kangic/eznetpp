// Copyright[2015] <kangic21@gmail.com>

#ifndef INCLUDE_RUNNER_H_
#define INCLUDE_RUNNER_H_

#include "./macros.h"

#include "../include/event_dispatcher.h"

namespace eznetpp {

class if_server;

class runner {
 public:
  runner(void);
  virtual ~runner(void);

 public:
  void register_server(if_server* listener);
  void deregister_server(if_server* listener);

  void run(void);

  // variables
 private:
  event_dispatcher evt_dispatcher;

  DISALLOW_COPY_AND_ASSIGN(runner);
};
}  // namespace eznetpp


#endif  // INCLUDE_RUNNER_H_

