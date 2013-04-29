#ifndef MESSAGE_READER_H
#define MESSAGE_READER_H

#include "zmqpp/zmqpp.hpp"
#include <boost/thread.hpp>
#include <map>

using namespace std;

class MessageReader {
public:
  MessageReader(zmqpp::context *ctx);
  ~MessageReader();
  void operator()();
private:
  bool running;
  zmqpp::context *context;
  boost::thread *my_thread;
  zmqpp::socket *msg_in_sock;
  zmqpp::socket *chan_actions_sock;
  map<string, string> users; // username <-> pubkey
};

#endif
