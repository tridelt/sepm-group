#include <iostream>
#include <boost/program_options.hpp>
#include "ExitHandler.h"
#include "IceServer.h"
#include "IceClient.h"
#include "MessageReader.h"
#include "SocketHandler.h"

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



  ExitHandler::i()->setHandler([](int) {
    // called when SIGINT (eg by Ctrl+C) is received
    // do cleanup

    // bad - cout not guaranteed to work, since not reentrant
    // this is just to show the handler is working
    cout << " Got signal .. terminating" << endl;
  });

  cout << "Hello from server" << endl;

  IceServer server;
  IceClient client("selinux.inso.tuwien.ac.at");

  MessageReader reader(SocketHandler::i()->getContext());

  string wait;
  cin >> wait;

  return 0;
}
