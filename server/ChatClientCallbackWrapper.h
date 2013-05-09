#ifndef CHAT_CLIENT_CALLBACK_WRAPPER_H
#define CHAT_CLIENT_CALLBACK_WRAPPER_H

#include "ChatClientCallbackInd.h"

class ChatClientCallbackWrapper : public virtual ChatClientCallbackInd {
public:
  ChatClientCallbackWrapper(sdc::ChatClientCallbackIPrx prx) : proxy(prx) {}

  void initChat(const sdc::StringSeq &users, const string &chat,
    const sdc::ByteSeq &key) {
    proxy->initChat(users, chat, key);
  }

  void addChatParticipant(const sdc::User &user, const string &chat) {
    proxy->addChatParticipant(user, chat);
  }

  void removeChatParticipant(const sdc::User &user, const string &chat) {
    proxy->removeChatParticipant(user, chat);
  }

  void appendMessageToChat(const sdc::ByteSeq &msg, const string &chat,
    const sdc::User &sender) {
    proxy->appendMessageToChat(msg, chat, sender);
  }

  std::string echo(const string &s) {
    return proxy->echo(s);
  }
private:
  sdc::ChatClientCallbackIPrx proxy;
};

#endif
