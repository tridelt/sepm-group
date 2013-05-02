#include <iostream>
#include <boost/program_options.hpp>
#include "ExitHandler.h"
#include "IceServer.h"
#include "IceClient.h"
#include "MessageReader.h"
#include "SocketHandler.h"
#include "Security.h"

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

  sdc::Security sec;
  sdc::ByteSeq pubkey, privkey;
  sec.genRSAKey(pubkey, privkey);
  auto key = sec.genAESKey(256);
  auto crypt_key = sec.encryptRSA(pubkey, key);


  IceServer server;
  //IceClient client("selinux.inso.tuwien.ac.at");

  MessageRouter router(SocketHandler::i()->getContext());
  MessageReader reader(SocketHandler::i()->getContext());

  cout << "Waiting for exit.." << endl;
  string wait;
  cin >> wait;

  return 0;
}
