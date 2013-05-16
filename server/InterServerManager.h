#ifndef INTER_SERVER_MANAGER_H
#define INTER_SERVER_MANAGER_H

#include "SecureDistributedChat.h"
#include <string>

using namespace std;
using namespace sdc;

class InterServerManager {
public:
  InterServerManager();
  Ice::ObjectPrx getServer(const string &hostname);
private:
  Ice::CommunicatorPtr ic;
  map <string, Ice::ObjectPrx> servers;
};

#endif
