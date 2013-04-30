#ifndef MESSAGE_ROUTER_H
#define MESSAGE_ROUTER_H

#include "zmqpp/zmqpp.hpp"
#include <boost/thread.hpp>

class MessageRouter {
public:
  MessageRouter(zmqpp::context* ctx);
  /* for message routing thread */
  void operator()();

  boost::thread *router_thread;
  zmqpp::context* context;
  bool running;
};

#endif
