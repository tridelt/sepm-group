#include "IceServer.h"
#include <IceSSL/IceSSL.h>
#include "SecureDistributedChat.h"
#include "AuthenticationImpl.h"
#include <IceUtil/IceUtil.h>

IceServer::IceServer(string pub_key_path, string priv_key_path, string ca_path) {
  int argc = 1;
  char prog_name[] = "sdc_client";
  char *argv[] = { prog_name };
  Ice::PropertiesPtr props = Ice::createProperties(argc, argv);
  try {
    // enable the SSL plugin for secure connections
    props->setProperty("Ice.Plugin.IceSSL", "IceSSL:createIceSSL");
    props->setProperty("IceSSL.CertFile", pub_key_path);
    props->setProperty("IceSSL.KeyFile", priv_key_path);
    props->setProperty("IceSSL.CertAuthFile", ca_path);
    props->setProperty("IceSSL.VerifyPeer", "1");
    props->setProperty("Ice.ThreadPool.Client.Size", "3");
    props->setProperty("Ice.ThreadPool.Server.Size", "3");
    // necessary so invalid connections will time out
    props->setProperty("Ice.Override.ConnectTimeout", "1000");
    // enable warnings
    // props->setProperty("Ice.Warn.Connections", "1");
    // props->setProperty("Ice.Trace.Protocol", "1");
    // props->setProperty("Ice.Trace.Network", "2");

    // Initialize a communicator with these properties.
    Ice::InitializationData id;
    id.properties = props;
    ic = Ice::initialize(id);

    Ice::ObjectAdapterPtr oa =
      ic->createObjectAdapterWithEndpoints("AuthenticationEndpoint", "ssl -p 1337");
    oa->add(new AuthenticationImpl(), ic->stringToIdentity("Authentication"));
    oa->activate();
  } catch (const Ice::Exception& e) {
    if (ic) ic->destroy();
    throw;
  }
}


IceServer::~IceServer() {
  if (ic) ic->destroy();
}


Ice::ObjectPrx IceServer::createAdapter(const Ice::ObjectPtr &o, const Ice::ConnectionPtr &) {
  // TODO: figure out how to properly create a new endpoint here
  // atm, this will only work for one client
  Ice::ObjectAdapterPtr oa = ic->createObjectAdapterWithEndpoints("SomeEndpoint", "ssl -p 1337");

  Ice::Identity adapter_ident;
  adapter_ident.name = IceUtil::generateUUID();
  adapter_ident.category = "";

  Ice::ObjectPrx proxy = oa->add(o, adapter_ident);
  oa->activate();

  return proxy;
}

void IceServer::run() {
  ic->waitForShutdown();
}

void IceServer::stop() {
  ic->destroy();
}
