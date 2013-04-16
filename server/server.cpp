#include <iostream>
#include <boost/program_options.hpp>
#include "ExitHandler.h"
#include "zmqpp/zmqpp.hpp"
#include "SecureDistributedChat.h"

namespace po = boost::program_options;
using namespace std;

int main(int argc, char** argv) {
  po::options_description desc("Allowed options");
  desc.add_options()
    ("help,h", "produce help message")
  ;

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);

  if(vm.count("help")) {
    cout << desc << "\n";
    return 1;
  }

  cout << "Hello from server" << endl;

  zmqpp::context context;

  zmqpp::socket message_source(context, zmqpp::socket_type::pub);
  message_source.bind("tcp://*:5555");

  zmqpp::socket message_sink(context, zmqpp::socket_type::sub);
  message_sink.bind ("tcp://*:5556");
  message_sink.subscribe("");

  ExitHandler::i()->setHandler([](int) {
    // called when SIGINT (eg by Ctrl+C) is received
    // do cleanup

    // bad - cout not guaranteed to work, since not reentrant
    // this is just to show the handler is working
    cout << " Got signal .. terminating" << endl;
  });

  try {
    while (true) {
      string msg;

      // wait for message
      message_sink.receive(msg);
      cout << "Received msg: [" << msg << "]" << endl;

      // Send reply back to client
      message_source.send(msg);
    }
  } catch (zmqpp::zmq_internal_exception) {
    cout << "Stopping zeromq.." << endl;
    message_source.close();
    message_sink.close();
  }

  return 0;
}
