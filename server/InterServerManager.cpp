#include "InterServerManager.h"
#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>
#include <stdexcept>

//TODO: destroy proxies on quit

InterServerManager::InterServerManager()
{
  //TODO: catch exceptions
  Ice::PropertiesPtr props = Ice::createProperties();
  props->setProperty("Ice.Plugin.IceSSL", "IceSSL:createIceSSL");
  //props->setProperty("IceSSL.CertAuthFile", this->cert);	//FIXME
  Ice::InitializationData id;
  id.properties = props;
  ic = Ice::initialize(id);
}

Ice::ObjectPrx InterServerManager::getServer(const string &hostname) {
  Ice::ObjectPrx s;
  try {
    s = servers.at(hostname);
  }
  catch(const out_of_range &e) {
    //TODO: catch errors
    s = ic->stringToProxy("InterServer:ssl -h " + hostname/* + "-p " + port*/);	//FIXME
    servers[hostname] = s;
  }
  return s;
}

