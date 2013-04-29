#ifndef SOCKET_HANDLER_H
#define SOCKET_HANDLER_H

#include "zmqpp/zmqpp.hpp"
#include <boost/thread/tss.hpp>

class SocketHandler {
public:
  static void create_sockets();
  static void destroy_sockets();

  static zmqpp::context *context;
private:
  static boost::thread_specific_ptr<zmqpp::socket> auth_req_sock;
  static boost::thread_specific_ptr<zmqpp::socket> msg_sock;
};


#endif
