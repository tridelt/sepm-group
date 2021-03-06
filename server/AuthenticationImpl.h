#ifndef AUTHENTIcATION_IMPL_H
#define AUTHENTIcATION_IMPL_H

#include "SecureDistributedChat.h"
#include "IceServerI.h"

using namespace std;

class DBPool;

class AuthenticationImpl : public virtual sdc::AuthenticationI {
public:
  AuthenticationImpl(IceServerI* s, DBPool *p) : server(s), db_pool(p) { }
  string echo(const string &s, const Ice::Current&) { return s; }
  void registerUser(const sdc::User &, const string &, const Ice::Current&);
  sdc::SessionIPrx login(const sdc::User &user, const string &pw,
    const Ice::Identity&, const Ice::Current&);
private:
  IceServerI *server;
  DBPool *db_pool;
};

#endif
