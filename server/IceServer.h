#ifndef ICE_CHAT_H
#define ICE_CHAT_H

#include <Ice/Ice.h>
#include <string>
using namespace std;

class IceServer {
public:
  IceServer(string pub_key_path = "server.crt",
            string priv_key_path = "server.key",
            string ca_path = "ca.crt");
  ~IceServer();

  void run();
  void stop();
  Ice::ObjectPrx createAdapter(const Ice::ObjectPtr &o, const Ice::ConnectionPtr &con);
private:
  Ice::CommunicatorPtr ic;
};

#endif
