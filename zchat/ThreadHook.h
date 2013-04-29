#include <iostream>
#include <Ice/Ice.h>
#include "SocketHandler.h"

using namespace std;
using namespace IceUtil;

class ThreadHook : public virtual Ice::ThreadNotification {
public:
  void start() {
    cout << "start: id = " << ThreadControl().id() << endl;
    SocketHandler::i()->create_sockets();
  }

  void stop() {
    cout << "stop: id = " << ThreadControl().id() << endl;
    SocketHandler::i()->destroy_sockets();
  }
};
