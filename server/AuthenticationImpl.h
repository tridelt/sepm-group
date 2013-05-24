#ifndef AUTHENTIcATION_IMPL_H
#define AUTHENTIcATION_IMPL_H

#include "SecureDistributedChat.h"
#include "IceServerI.h"
#include "ChatManager.h"
#include <memory>

using namespace std;

class DBPool;

class AuthenticationImpl : public virtual sdc::AuthenticationI {
public:
  AuthenticationImpl(IceServerI* s, shared_ptr<DBPool> p,
    shared_ptr<ChatManager> mgr) :
    server(s), db_pool(p), chat_mgr(mgr) { }
  string echo(const string &s, const Ice::Current&) { return s; }
  void registerUser(const sdc::User &, const string &, const Ice::Current&);
  sdc::SessionIPrx login(const sdc::User &user, const string &pw,
    const Ice::Identity&, const Ice::Current&);
private:
  IceServerI* server;
  shared_ptr<DBPool> db_pool;
  shared_ptr<ChatManager> chat_mgr;
};

#endif
