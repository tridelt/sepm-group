#include <iostream>
#include <boost/program_options.hpp>
#include "ExitHandler.h"
#include "SecureDistributedChat.h"
#include "IceServer.h"
#include "SessionImpl.h"
#include "Logging.h"
#include "config.h"

namespace po = boost::program_options;
using namespace std;

int main(int argc, char** argv) {
  po::options_description desc("Allowed options");
  desc.add_options()
    ("quiet,q", "don't log to stdout")
    ("help,h", "produce help message")
  ;

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);

  if(vm.count("help")) {
    cout << desc << "\n";
    return 1;
  }

  if(vm.count("quiet")) {
    logger.clearSinks();
  }

  FileSink fileSink(true, Severity::LVL_INFO, "server.log");
  logger.addSink(&fileSink);

  INFO("Hello from server");
  WARN("LOOK");
  ERROR("pretty colors!");
  INFO("my hostname: ", Config::hostname());


  IceServer server;

  ExitHandler::i()->setHandler([&server](int sig) {
    // called when SIGINT (eg by Ctrl+C) is received
    // do cleanup

    server.exit();

    // bad - cout not guaranteed to work, since not reentrant
    // this is just to show the handler is working
    INFO("Got signal ", sig, " .. terminating");
  });

  server.wait();

  INFO("normal exit");

  return 0;
}
