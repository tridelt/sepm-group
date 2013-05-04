#include <iostream>
#include <boost/program_options.hpp>
#include "ExitHandler.h"
#include "SecureDistributedChat.h"
#include "IceServer.h"

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


  ExitHandler::i()->setHandler([](int) {
    // called when SIGINT (eg by Ctrl+C) is received
    // do cleanup

    // bad - cout not guaranteed to work, since not reentrant
    // this is just to show the handler is working
    cout << " Got signal .. terminating" << endl;
  });

  IceServer server();

  string wait;
  cin >> wait;

  return 0;
}
