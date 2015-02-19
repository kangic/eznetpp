// Copyright[2015] <kangic21@gmail.com>
#ifndef SRC_RUNNER_H_
#define SRC_RUNNER_H_

#include "./macros.h"

#include "../include/event_dispatcher.h"

namespace eznetpp {

class if_event_listener;

class runner {
public:
  runner(void);
  virtual ~runner(void);

public:
  void register_event_listener(if_event_listener* listener);
  void deregister_event_listener(if_event_listener* listener);
  void run(void);

private:
  DISALLOW_COPY_AND_ASSIGN(runner);

	// variables
private:
	event_dispatcher evt_dispatcher;
};
}  // namespace eznetpp


#endif  // SRC_RUNNER_H_

