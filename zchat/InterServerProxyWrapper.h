#ifndef INTERSERVER_PROXY_WRAPPER_IMPL_H
#define INTERSERVER_PROXY_WRAPPER_IMPL_H


#include "SecureDistributedChat.h"
#include <string>

using namespace std;

class InterServerProxyWrapper : public virtual sdc::InterServerI {
public:
  InterServerProxyWrapper(sdc::InterServerIPrx prx) : proxy(prx) { }

  sdc::User retrieveUser(const string &user, const Ice::Current &) {
    return proxy->retrieveUser(user);
  }

  void invite(const sdc::User &recipient, const string &chat,
    const sdc::ByteSeq &encryptedKey, const Ice::Current &) {
    proxy->invite(recipient, chat, encryptedKey);
  }

  void sendMessage(const sdc::User &sender, const sdc::ByteSeq &msg,
    const string &chat, const Ice::Current &) {
    proxy->sendMessage(sender, msg, chat);
  }

  void leaveChat(const sdc::User &user, const string &chat, const Ice::Current&) {
    proxy->leaveChat(user, chat);
  }

  void clientInitChat(const sdc::User &user, const sdc::StringSeq &nicklist,
    const string &chan, const sdc::ByteSeq &key, const Ice::Current &) {
    proxy->clientInitChat(user, nicklist, chan, key);
  }

  void clientAddChatParticipant(const sdc::User &recipient,
    const sdc::User &joining_user, const string &chat, const Ice::Current &) {
    proxy->clientAddChatParticipant(recipient, joining_user, chat);
  }

  void clientRemoveChatParticipant(const sdc::User &recipient,
    const sdc::User &leaving_user, const string &chat, const Ice::Current &) {
    proxy->clientRemoveChatParticipant(recipient, leaving_user, chat);
  }

  void clientAppendMessageToChat(const sdc::User &recipient,
      const sdc::ByteSeq &msg, const string &chat, const sdc::User &sender,
      const Ice::Current &) {
    proxy->clientAppendMessageToChat(recipient, msg, chat, sender);
  }

private:
  sdc::InterServerIPrx proxy;
};

#endif
