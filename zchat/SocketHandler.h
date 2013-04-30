#ifndef SOCKET_HANDLER_H
#define SOCKET_HANDLER_H

#include "zmqpp/zmqpp.hpp"
#include <boost/thread/tss.hpp>
#include <boost/thread.hpp>
#include <string>
#include <set>
#include "MessageRouter.h"

using namespace std;

class SocketHandler {
public:
  static SocketHandler* i();

  void create_sockets();
  void destroy_sockets();
  void destroy_all_sockets();

  void send_msg(string chat, string recipient, string sender, string msg);
  void send_chan_event(string chat, string recipient, string event, string body);

  zmqpp::context* getContext() { return &context; }
private:
  SocketHandler() { }
  ~SocketHandler();
  static SocketHandler* instance;
  static boost::mutex mutex;

  zmqpp::context context;
  boost::thread_specific_ptr<zmqpp::socket> chan_event_sock;
  boost::thread_specific_ptr<zmqpp::socket> msg_sock_out;
  set<zmqpp::socket*> sockets;
};


#endif
