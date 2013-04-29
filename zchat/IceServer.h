#include <Ice/Ice.h>
#include <string>

using namespace std;

class IceServer {
  public:
    IceServer(int port = 1337, string pub_key_path = "server.crt",
      string priv_key_path = "server.key", string ca_path = "ca.crt");
    ~IceServer();

    void run();
    void stop();
  private:
    Ice::CommunicatorPtr ic;
};
