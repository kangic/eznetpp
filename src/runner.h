// Copyright[2015] <kangic21@gmail.com>
#ifndef SRC_RUNNER_H_
#define SRC_RUNNER_H_

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


#endif  // SRC_RUNNER_H_

