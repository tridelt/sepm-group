#ifndef SESSION_IMPL_H
#define SESSION_IMPL_H

#include "SecureDistributedChat.h"
#include "ChatManager.h"
#include "ChatClientCallbackInd.h"
using namespace std;

class DBPool;

class SessionImpl : public virtual sdc::SessionI {
public:
  SessionImpl(sdc::User, DBPool *p, ChatManager *mgr, shared_ptr<ChatClientCallbackInd> cb);
  void logout(const Ice::Current&);
  sdc::User retrieveUser(const string&, const Ice::Current&);
  std::string initChat(const Ice::Current&);
  void leaveChat(const string &chat, const Ice::Current&);
  void invite(const sdc::User &other, const string &chat, const sdc::ByteSeq &key, const Ice::Current&);
  void sendMessage(const sdc::ByteSeq &msg, const string &chat, const Ice::Current&);
  void deleteUser(const sdc::User &user, const Ice::Current&);
  void saveLog(const string&, Ice::Long, const sdc::SecureContainer&, const Ice::Current&);
  sdc::Loglist retrieveLoglist(const Ice::Current&);
  sdc::SecureContainer retrieveLog(const string&, Ice::Long, const Ice::Current&);
  void saveContactList(const sdc::SecureContainer&, const Ice::Current&);
  sdc::SecureContainer retrieveContactList(const Ice::Current&);
  sdc::User getUser() const { return user; }
  shared_ptr<ChatClientCallbackInd> getCallback() const { return callback; }
private:
  bool loggedIn;
  sdc::User user;
  DBPool *db_pool;
  ChatManager *chat_mgr;
  shared_ptr<ChatClientCallbackInd> callback;
};

#endif
