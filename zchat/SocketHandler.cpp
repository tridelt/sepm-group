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

  auth_req_sock.reset(new zmqpp::socket(context, zmqpp::socket_type::req));
  auth_req_sock->set(zmqpp::socket_option::linger, 10);
  auth_req_sock->connect("ipc://sepm_auth_req");

  msg_sock.reset(new zmqpp::socket(context, zmqpp::socket_type::pub));
  msg_sock->set(zmqpp::socket_option::linger, 10);
  msg_sock->connect("ipc://sepm_msg");

  sockets.insert(auth_req_sock.get());
  sockets.insert(msg_sock.get());
}

void SocketHandler::destroy_sockets() {
  boost::lock_guard<boost::mutex> lock(mutex);

  if(sockets.count(auth_req_sock.get()) != 0) {
    sockets.erase(auth_req_sock.get());
    auth_req_sock->close();
    auth_req_sock.release();
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

void SocketHandler::send_msg(string chat, string user, string msg) {
  zmqpp::message zmsg;
  zmsg << chat << user << msg;
  msg_sock->send(zmsg);
}
