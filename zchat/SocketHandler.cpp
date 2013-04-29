#include "SocketHandler.h"
#include <string>
#include <boost/thread.hpp>
#include "defines.h"

using namespace std;

SocketHandler* SocketHandler::instance;
boost::mutex SocketHandler::mutex;

SocketHandler* SocketHandler::i() {
  if(instance == NULL) {
    boost::lock_guard<boost::mutex> lock(mutex);
    if(instance == NULL)
      instance = new SocketHandler();
  }
  return instance;
}

SocketHandler::~SocketHandler() {
  destroy_all_sockets();
}

void SocketHandler::create_sockets() {
  boost::lock_guard<boost::mutex> lock(mutex);

  chan_event_sock.reset(new zmqpp::socket(context, zmqpp::socket_type::pub));
  chan_event_sock->set(zmqpp::socket_option::linger, 10);
  chan_event_sock->bind(ZMQ_SOCK_CHAN_EVENT_BIND);

  msg_sock_out.reset(new zmqpp::socket(context, zmqpp::socket_type::pub));
  msg_sock_out->set(zmqpp::socket_option::linger, 10);
  msg_sock_out->bind(ZMQ_SOCK_MSG_OUT_BIND);

  sockets.insert(chan_event_sock.get());
  sockets.insert(msg_sock_out.get());
}

void SocketHandler::destroy_sockets() {
  boost::lock_guard<boost::mutex> lock(mutex);

  if(sockets.count(chan_event_sock.get()) != 0) {
    sockets.erase(chan_event_sock.get());
    chan_event_sock->close();
    chan_event_sock.release();
  }

  if(sockets.count(msg_sock_out.get()) != 0) {
    sockets.erase(msg_sock_out.get());
    msg_sock_out->close();
    msg_sock_out.release();
  }
}

void SocketHandler::destroy_all_sockets() {
  boost::lock_guard<boost::mutex> lock(mutex);

  for(auto s : sockets) {
    s->close();
  }

  sockets.clear();
}

void SocketHandler::send_msg(string chat, string recipient, string sender, string msg) {
  zmqpp::message zmsg;
  zmsg << chat << recipient << sender << msg;
  msg_sock_out->send(zmsg);
}


void SocketHandler::send_chan_event(string chat, string recipient, string event,
  string body) {
  zmqpp::message zmsg;
  zmsg << chat << recipient << event << body;
  chan_event_sock->send(zmsg);
}
