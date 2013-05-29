#ifndef INTER_SERVER_MANAGER_H
#define INTER_SERVER_MANAGER_H

#include "SecureDistributedChat.h"
#include <boost/shared_ptr.hpp>
#include <string>

using namespace std;
using namespace sdc;

class InterServerManager {
public:
  InterServerManager(Ice::CommunicatorPtr server_ic);
  InterServerIPrx getServerForID(const string &id);
private:
  Ice::CommunicatorPtr ic;
  map <string, InterServerIPrx> servers;
};

#endif
