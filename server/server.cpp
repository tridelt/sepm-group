#include <iostream>
#include <boost/program_options.hpp>
#include "ExitHandler.h"
#include "SecureDistributedChat.h"
#include "IceServer.h"
#include "SessionImpl.h"
#include "Logging.h"

namespace po = boost::program_options;
using namespace std;

int main(int argc, char** argv) {
  FileSink fileSink(true, Severity::LVL_INFO, "server.log");
  DashSink dashSink(true, Severity::LVL_INFO);
  logger.addSink(&fileSink);
  logger.addSink(&dashSink);

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


  INFO("Hello from server");
  WARN("LOOK");
  ERROR("pretty colors!");


  ExitHandler::i()->setHandler([](int) {
    // called when SIGINT (eg by Ctrl+C) is received
    // do cleanup

    // bad - cout not guaranteed to work, since not reentrant
    // this is just to show the handler is working
    INFO(" Got signal .. terminating");
  });

  IceServer server;

  sdc::User u;
  SessionImpl session(u);

  server.exposeObject(&session);

  string wait;
  cin >> wait;

  return 0;
}
