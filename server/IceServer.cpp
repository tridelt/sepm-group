#include "IceServer.h"
#include <IceSSL/IceSSL.h>
#include "SecureDistributedChat.h"
#include "AuthenticationImpl.h"
#include "ThreadHook.h"
#include "ChatClientCallbackWrapper.h"

IceServer::IceServer(string pub_key_path, string priv_key_path, string ca_path) {
  db_pool = DBPool::ProdPool();
  chat_mgr = new ChatManager();

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

    oa = ic->createObjectAdapterWithEndpoints("AuthenticationEndpoint", "ssl -p 1337");
    oa->add(new AuthenticationImpl(this, db_pool, chat_mgr), ic->stringToIdentity("Authentication"));
    oa->activate();
  } catch (const Ice::Exception& e) {
    if (ic) ic->destroy();
    throw;
  }
}


IceServer::~IceServer() {
  delete db_pool;
  delete chat_mgr;
  // TODO: investigate crash on ic->destroy()
  // calling this for some reason causes the server to crash
  // if (ic) ic->destroy();
}


Ice::ObjectPrx IceServer::exposeObject(const Ice::ObjectPtr &o, const string &name) {
  Ice::Identity adapter_ident;
  adapter_ident.name = name;
  adapter_ident.category = "";

  Ice::ObjectPrx proxy = oa->add(o, adapter_ident);
  oa->activate();

  return proxy;
}


shared_ptr<ChatClientCallbackInd> IceServer::callbackForID(const Ice::Identity &callbackID,
                                const Ice::ConnectionPtr &con) {
  Ice::ObjectPrx callbackProxy = con->createProxy(callbackID);
  return shared_ptr<ChatClientCallbackInd>(
    new ChatClientCallbackWrapper(sdc::ChatClientCallbackIPrx::checkedCast(callbackProxy)));
}
