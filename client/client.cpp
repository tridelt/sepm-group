#include <iostream>
#include <boost/program_options.hpp>
#include <boost/bind.hpp>
#include "FileWatcher.h"
#include "ViewRefresher.h"
#include "ExitHandler.h"
#include "PluginManager.h"
#include "zmqpp/zmqpp.hpp"
#include <QtConcurrentRun>

namespace po = boost::program_options;
using namespace std;

class MessageReader {
public:
  MessageReader(zmqpp::socket *s) {
    QtConcurrent::run(this, &MessageReader::read, s);
  }

  void read(zmqpp::socket *source) {
    while(true) {
      string msg;
      source->receive(msg);
      cout << "> " << msg << endl;
    }
  }
};

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


  try {

    ViewRefresher refresher;
    FileWatcher watcher("./ui", boost::bind(&ViewRefresher::fileChanged, &refresher, _1, _2, _3));

    // create or modify a file in ./ui to see the changes getting picked up
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

    ExitHandler::i()->setHandler([](int) {
      // called when SIGINT (eg by Ctrl+C) is received
      // do cleanup

      // bad - cout not guaranteed to work, since not reentrant
      // this is just to show the handler is working
      cout << " Got signal .. terminating" << endl;
    });

    PluginManager manager;

    manager.listPlugins();


    zmqpp::context context;

    zmqpp::socket message_source(context, zmqpp::socket_type::sub);
    message_source.connect("tcp://127.0.0.1:5555");
    message_source.subscribe("");

    zmqpp::socket message_sink(context, zmqpp::socket_type::pub);
    message_sink.connect("tcp://127.0.0.1:5556");

    MessageReader reader(&message_source);

    string line;

    while(true) {
      getline(cin, line);
      if(cin.eof())
        break;
      message_sink.send(line);
    }

  } catch(PluginException ex) {
    cerr << ex.what() << endl;
  }



  return 0;
}
