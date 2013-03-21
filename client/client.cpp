#include <iostream>
#include <boost/program_options.hpp>
#include "FileWatcher.h"
#include "ViewRefresher.h"

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

  cout << "Hello from client" << endl;

  ViewRefresher refresher;
  FileWatcher watcher("./ui", bind(&ViewRefresher::refreshAll, &refresher));

  return 0;
}
