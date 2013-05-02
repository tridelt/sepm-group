#include "MessageReader.h"
#include "defines.h"
#include "ChatDB.h"
#include "SocketHandler.h"
#include "Security.h"

MessageReader::MessageReader(zmqpp::context *ctx) : running(true), context(ctx) {
  my_thread = new boost::thread(*this);
}

MessageReader::~MessageReader() {
  running = false;
}

void MessageReader::operator()() {
  cout << "starting MessageReader thread" << endl;

  SocketHandler::i()->create_sockets();

  cout << "binding chan_actions_sock to " << ZMQ_SOCK_CHAN_ACTION_BIND << endl;
  chan_actions_sock = new zmqpp::socket(*context, zmqpp::socket_type::sub);
  chan_actions_sock->set(zmqpp::socket_option::linger, 10);
  chan_actions_sock->subscribe("");
  chan_actions_sock->bind(ZMQ_SOCK_CHAN_ACTION_BIND);

  cout << "binding msg_in_sock to " << ZMQ_SOCK_MSG_IN_BIND << endl;
  msg_in_sock = new zmqpp::socket(*context, zmqpp::socket_type::sub);
  msg_in_sock->set(zmqpp::socket_option::linger, 10);
  msg_in_sock->subscribe("");
  msg_in_sock->bind(ZMQ_SOCK_MSG_IN_BIND);

  zmqpp::poller poller;
  poller.add(*chan_actions_sock);
  poller.add(*msg_in_sock);

  try {
    while(running) {
      if(!poller.poll(100 /* ms */))
        continue;

      if(poller.has_input(*msg_in_sock)) {
        zmqpp::message msg;
        msg_in_sock->receive(msg);

        if(msg.parts() == 3) {
          string chan, user, content;
          msg >> chan;
          msg >> user;
          msg >> content;
          cout << "[" << chan << "] " << user << ": " << content << endl;
          auto sender = ChatDB::i()->userForString(user);
          auto server = ChatDB::i()->serverForChat(chan);
          auto cryptText = ChatDB::i()->encryptMsgForChat(chan, content);
          server->sendMessage(sender, cryptText, chan);
        } else {
          cout << "got unknown msg: ";
          for(uint i = 0; i < msg.parts(); ++i) {
            cout << msg.get(i) << " ";
          }
          cout << endl;
        }
      }

      if(poller.has_input(*chan_actions_sock)) {
        zmqpp::message msg;
        chan_actions_sock->receive(msg);

        if(msg.parts() >= 3) {
          string chan, user, action;
          msg >> chan;
          msg >> user;
          msg >> action;

          auto sender = ChatDB::i()->userForString(user);
          auto server = ChatDB::i()->serverForUser(user);

          if(action == "join") {
            // only works with local chats
            cout << user << " joined " << chan << endl;
            ChatDB::i()->addUserToChat(chan, user);
          } else if(action == "quit") {
            cout << user << " quit " << chan << endl;
            server->leaveChat(sender, chan);
          } else if(action == "invite") {
            cout << user << " was invited to " << chan << endl;

            sdc::Security sec;
            sec.savePubKey(sender.publicKey, "pubkey1");

            auto raw_key = ChatDB::i()->getKeyForChat(chan);
            auto key = sec.encryptRSA(sender.publicKey, raw_key);
            server->invite(sender, chan, key);
          } else {
            cout << "unknown action " << action << endl;
          }

        } else {
          cout << "got unknown msg: ";
          for(uint i = 0; i < msg.parts(); ++i) {
            cout << msg.get(i) << " ";
          }
          cout << endl;
        }
      }
    }
  } catch(zmqpp::zmq_internal_exception &e) {
    cout << "error: " << e.zmq_error() << endl;
  }

  delete msg_in_sock;
  delete chan_actions_sock;

  SocketHandler::i()->destroy_sockets();
  cout << "stopping MessageReader thread" << endl;
}
