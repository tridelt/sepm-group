#ifndef ICE_CLIENT_H
#define ICE_CLIENT_H

#include <boost/format.hpp>
#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>
#include <IceSSL/IceSSL.h>
#include "SecureDistributedChat.h"
#include "ThreadHook.h"
#include "InterServerProxyWrapper.h"

using namespace std;

// wrapper for Ice boilerplate
class IceClient {
public:
  // constructor tries to set up connection to server, throws if it fails
  IceClient(string server, int port = 1337, string cert_path = "ca.crt") {
    boost::format connection_string = boost::format("InterServer:ssl -h %1% -p %2%");
    int argc = 1;
    char prog_name[] = "sdc_client";
    char *argv[] = { prog_name };
    Ice::PropertiesPtr props = Ice::createProperties(argc, argv);
    try {
      // enable the SSL plugin for secure connections
      props->setProperty("Ice.Plugin.IceSSL", "IceSSL:createIceSSL");
      props->setProperty("IceSSL.CertAuthFile", cert_path);
      props->setProperty("Ice.ACM.Client", "0");
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

      Ice::ObjectPrx base = ic->stringToProxy(
        (connection_string % server % port).str() );

      interServer = sdc::InterServerIPrx::checkedCast(base);
      wrapper = new InterServerProxyWrapper(interServer);
    } catch (const sdc::SDCException &e) {
      cout << "got exception: " << e.what << endl;
      if (ic) ic->destroy();
      throw;
    } catch (const Ice::Exception& e) {
      if (ic) ic->destroy();
      throw;
    }
  }

  // automatically close connection on destruction
  ~IceClient() {
    if (ic) ic->destroy();
  }

  sdc::InterServerI* getServer() {
    return wrapper;
  }

private:
  Ice::CommunicatorPtr ic;
  sdc::InterServerIPrx interServer;
  InterServerProxyWrapper* wrapper;
};

#endif
