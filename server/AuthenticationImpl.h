#ifndef AUTHENTIcATION_IMPL_H
#define AUTHENTIcATION_IMPL_H

#include "SecureDistributedChat.h"
#include "IceServer.h"
#include "ChatManager.h"
#include "SessionManager.h"
#include <memory>

using namespace std;

class DBPool;

class AuthenticationImpl : public virtual sdc::AuthenticationI {
public:
  AuthenticationImpl(shared_ptr<IceServer> s) : server(s) { }
  string echo(const string &s, const Ice::Current&) { return s; }
  void registerUser(const sdc::User &, const string &, const Ice::Current&);
  sdc::SessionIPrx login(const sdc::User &user, const string &pw,
    const Ice::Identity&, const Ice::Current&);
private:
  shared_ptr<IceServer> server;
};

#endif
