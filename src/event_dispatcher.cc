#include "../include/event_dispatcher.h"

#include <cstdio>

#include "../include/if_event_listener.h"

namespace eznetpp {

event_dispatcher::event_dispatcher(void) {
  listener_container_.clear();
}

event_dispatcher::~event_dispatcher(void) {
  listener_container_.clear();
}

bool event_dispatcher::reg_event_listener(
    if_event_listener* listener) {
  // step 1. check to exist object.
  std::vector<if_event_listener*>::iterator iter;
  for (iter = listener_container_.begin();
       iter != listener_container_.end();
       ++iter) {
    if ( (*iter) == listener )
      return false;
  }

  // step 2. add listener to the listener_container
  listener_container_.push_back(listener);

  return true;
}

bool event_dispatcher::dereg_event_listener(
    if_event_listener* listener) {
  std::vector<if_event_listener*>::iterator iter;
  for (iter = listener_container_.begin();
       iter != listener_container_.end();
       ++iter) {
    if ( (*iter) == listener ) {
      iter = listener_container_.erase(iter);
      return true;
    }
  }

  return false;
}



}  // namespace eznetpp
