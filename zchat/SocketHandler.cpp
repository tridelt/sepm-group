#include "SocketHandler.h"
#include <string>
#include <boost/thread.hpp>

using namespace std;

SocketHandler* SocketHandler::instance;
boost::mutex SocketHandler::mutex;

SocketHandler* SocketHandler::i() {
  boost::lock_guard<boost::mutex> lock(mutex);
  if(instance == NULL) {
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
  chan_event_sock->connect("ipc://sepm_chan_event");

  msg_sock.reset(new zmqpp::socket(context, zmqpp::socket_type::pub));
  msg_sock->set(zmqpp::socket_option::linger, 10);
  msg_sock->connect("ipc://sepm_msg");

  sockets.insert(chan_event_sock.get());
  sockets.insert(msg_sock.get());
}

void SocketHandler::destroy_sockets() {
  boost::lock_guard<boost::mutex> lock(mutex);

  if(sockets.count(chan_event_sock.get()) != 0) {
    sockets.erase(chan_event_sock.get());
    chan_event_sock->close();
    chan_event_sock.release();
  }

  if(sockets.count(msg_sock.get()) != 0) {
    sockets.erase(msg_sock.get());
    msg_sock->close();
    msg_sock.release();
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
  msg_sock->send(zmsg);
}


void SocketHandler::send_chan_event(string chat, string recipient, string event,
  string body) {
  zmqpp::message zmsg;
  zmsg << chat << recipient << event << body;
  chan_event_sock->send(zmsg);
}
