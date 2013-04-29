#include "IceServer.h"
#include "ThreadHook.h"
#include "InterServerImpl.h"

IceServer::IceServer(int port, string pub_key_path, string priv_key_path, string ca_path) {
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
    id.threadHook = new ThreadHook();
    ic = Ice::initialize(id);

    Ice::ObjectAdapterPtr oa =
      ic->createObjectAdapterWithEndpoints("InterServerEndpoint", "ssl -p " + port);
    oa->add(new InterServerImpl(), ic->stringToIdentity("InterServer"));
    oa->activate();
  } catch (const Ice::Exception& e) {
    if (ic) ic->destroy();
    throw;
  }
}


IceServer::~IceServer() {
  if (ic) ic->destroy();
}
