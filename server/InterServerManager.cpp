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

InterServerManager::InterServerManager(const string &cafile)
{
  try {
    Ice::PropertiesPtr props = Ice::createProperties();
    props->setProperty("Ice.Plugin.IceSSL", "IceSSL:createIceSSL");
    props->setProperty("IceSSL.CertAuthFile", cafile);
    Ice::InitializationData id;
    id.properties = props;
    id.threadHook = new ThreadHook(); 
    ic = Ice::initialize(id);
  } catch(...) {
    ERROR("failed to create Ice connection for InterServerManager");
  }
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

