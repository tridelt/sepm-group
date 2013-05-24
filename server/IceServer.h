#ifndef ICE_CHAT_H
#define ICE_CHAT_H

#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>
#include <string>
#include "DBPool.h"
#include "IceServerI.h"
#include "ChatManager.h"

using namespace std;

class IceServer : public virtual IceServerI {
public:
  /**
   * create a new ice server
   */
  IceServer();
  ~IceServer();
  
  
  
  /**
   * initialize the new ice server, listening on port 1337
   *
   * sets up the necessary options for ssl connections and then exposes
   * the AuthenticationI interface
   *
   * @param pub_key_path    the path to the public key of the server
   * @param priv_key_path   the path to the private key of the server
   * @param ca_path         the path to the certificate authority file, used by
   *                        all groups
   */
  void init(string pub_key_path = "server.crt",
            string priv_key_path = "server.key",
            string ca_path = "ca.crt");

  /**
   * expose an object over the object adapter under an optional name
   *
   * uses the object adapter created when constructing this class (which is
   * listening on port 1337 for ssl connections) to expose the provided object
   * o. If no name is provided, a random UUID is generated
   *
   * @param  o    the object to expose
   * @param  name the name to expose it under, randomly chosen if not provided
   * @return      a proxy for the exposed object
   */
  Ice::ObjectPrx exposeObject(const Ice::ObjectPtr &o,
                              const string &name = IceUtil::generateUUID());



  shared_ptr<ChatClientCallbackInd> callbackForID(const Ice::Identity &callbackID,
                                const Ice::ConnectionPtr &con);

  /**
   * hangs until the ice server has shutdown
   */
  void wait() {
    ic->waitForShutdown();
  }

  /**
   * tells the ice server to shutdown
   */
  void exit() {
    ic->shutdown();
  }
private:
  bool initialized;
  Ice::CommunicatorPtr ic;
  Ice::ObjectAdapterPtr oa;
  DBPool *db_pool;
  ChatManager *chat_mgr;
};

#endif
