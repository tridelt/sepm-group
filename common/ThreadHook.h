#ifndef THREAD_HOOK_H
#define THREAD_HOOK_H

#include <iostream>
#include <Ice/Ice.h>
#include "Logging.h"

using namespace std;
using namespace IceUtil;

class ThreadHook : public virtual Ice::ThreadNotification {
public:
  void start() {
    logger.registerThread();
    INFO("howdy from ",ThreadControl().id());
  }

  void stop() {
    INFO("adios from ",ThreadControl().id());
    logger.removeThread();
  }
};

#endif
