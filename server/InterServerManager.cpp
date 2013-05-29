#include "InterServerManager.h"
#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>
#include <stdexcept>
#include "sdcHelper.h"
#include "Logging.h"
#include "config.h"
#include "ThreadHook.h"
#include "InterServerImpl.h"

//TODO: call destroy on communicator when server quits

InterServerManager::InterServerManager(Ice::CommunicatorPtr server_ic)
  : ic(server_ic) {
}

InterServerIPrx InterServerManager::getServerForID(const string &id) {
  if(!sdc::sdcHelper::isValidID(id)) {
    WARN("attempting to get server from invalid ID ", id);
    throw;
  }

  string hostname = sdc::sdcHelper::getServerFromID(id);

  InterServerIPrx s;
  if(servers.count(hostname) == 1) s = servers[hostname];
  else {
    try {
      Ice::ObjectPrx op = ic->stringToProxy("InterServer:ssl -h " + hostname + " -p 1337");
      s = InterServerIPrx::checkedCast(op);
    } catch(const Ice::Exception& e) {
      ERROR("failed to connect to remote server ", hostname, ": ", e.ice_name(), "(\"InterServer:ssl -h " + hostname + "-p 1337\")");
    }
    servers[hostname] = s;
  }
  return s;
}

