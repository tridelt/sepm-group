#include <iostream>
#include <boost/program_options.hpp>
#include <boost/bind.hpp>
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
  FileWatcher watcher("./ui", boost::bind(&ViewRefresher::fileChanged, &refresher, _1, _2, _3));

  FileWatcher demo("./ui", [](string name, bool isDir, FileWatcher::FileEvent e) {
    switch(e) {
      case FileWatcher::CREATE:
        cout << "created ";
        break;
      case FileWatcher::MODIFY:
        cout << "modified ";
        break;
      case FileWatcher::DELETE:
        cout << "deleted ";
        break;
      default:
        cout << "unknown event: ";
    }
    cout << (isDir ? "dir " : "file ");
    cout << name << endl;
  });

  int test;
  cin >> test;

  return 0;
}
