#ifndef _EZNETPP_SRC_RUNNER_H_
#define _EZNETPP_SRC_RUNNER_H_

#include "./macros.h"

namespace eznetpp {

class runner {
 public:
  runner(void);
  virtual ~runner(void);

 public:
  void run(void);

 private:
  DISALLOW_COPY_AND_ASSIGN(runner);
};
}  // namespace eznetpp


#endif  // _EZNETPP_SRC_RUNNER_H_

