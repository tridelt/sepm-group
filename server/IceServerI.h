#ifndef ICE_SERVER_I_H
#define ICE_SERVER_I_H

#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>
#include <string>
#include <boost/shared_ptr.hpp>
#include "ChatClientCallbackInd.h"

using namespace std;

class IceServerI {
public:
  virtual Ice::ObjectPrx exposeObject(const Ice::ObjectPtr &o,
                                const string &name = IceUtil::generateUUID()) = 0;
  virtual shared_ptr<ChatClientCallbackInd> callbackForID(const Ice::Identity &callbackID,
                                const Ice::ConnectionPtr &con) = 0;
  virtual ~IceServerI() {}
};

#endif
