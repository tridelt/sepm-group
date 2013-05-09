#ifndef ICE_SERVER_I_H
#define ICE_SERVER_I_H

#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>
#include <string>

using namespace std;

class IceServerI {
public:
  virtual Ice::ObjectPrx exposeObject(const Ice::ObjectPtr &o,
                                const string &name = IceUtil::generateUUID()) = 0;
  virtual ~IceServerI() {}
};

#endif
