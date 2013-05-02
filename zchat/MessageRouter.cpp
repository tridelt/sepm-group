#include "MessageRouter.h"
#include "defines.h"
#include <iostream>
#include <zmq.h>

using namespace std;

MessageRouter::MessageRouter(zmqpp::context *ctx) : context(ctx), running(true) {
  router_thread = new boost::thread(*this);
}

void MessageRouter::operator()() {
  cout << "starting MessageRouter routing thread" << endl;

  zmqpp::socket chan_event_sock_int(*context, zmqpp::socket_type::sub);
  chan_event_sock_int.set(zmqpp::socket_option::linger, 10);
  chan_event_sock_int.subscribe("");
  chan_event_sock_int.bind(ZMQ_SOCK_CHAN_EVENT_BIND_INTERNAL);

  zmqpp::socket chan_event_sock(*context, zmqpp::socket_type::pub);
  chan_event_sock.set(zmqpp::socket_option::linger, 10);
  chan_event_sock.bind(ZMQ_SOCK_CHAN_EVENT_BIND);


  zmqpp::socket msg_sock_out_int(*context, zmqpp::socket_type::sub);
  msg_sock_out_int.set(zmqpp::socket_option::linger, 10);
  msg_sock_out_int.subscribe("");
  msg_sock_out_int.bind(ZMQ_SOCK_MSG_OUT_BIND_INTERNAL);

  zmqpp::socket msg_sock_out(*context, zmqpp::socket_type::pub);
  msg_sock_out.set(zmqpp::socket_option::linger, 10);
  msg_sock_out.bind(ZMQ_SOCK_MSG_OUT_BIND);


  zmqpp::poller poller;
  poller.add(chan_event_sock_int);
  poller.add(msg_sock_out_int);

  while(running) {
    try {
      if(!poller.poll(100 /* ms */))
        continue;

      if(poller.has_input(msg_sock_out_int)) {
        zmqpp::message msg;
        msg_sock_out_int.receive(msg);
        msg_sock_out.send(msg);
      }

      if(poller.has_input(chan_event_sock_int)) {
        zmqpp::message msg;
        chan_event_sock_int.receive(msg);
        chan_event_sock.send(msg);
      }
    }
    catch(zmqpp::zmq_internal_exception &e) {
      cout << "error " << e.zmq_error() << ": " << zmq_strerror(e.zmq_error()) << endl;
      if(e.zmq_error() == 4)
        continue;
      else
        break;
    }
  }

  cout << "stopping MessageRouter routing thread" << endl;
}
