#ifndef CHAT_CLIENT_CALLBACK_IND_H
#define CHAT_CLIENT_CALLBACK_IND_H

#include "SecureDistributedChat.h"
#include <string>

using namespace std;

class ChatClientCallbackInd {
public:
  virtual void initChat(const sdc::StringSeq &, const string &,
    const sdc::ByteSeq &) {}

  virtual void addChatParticipant(const sdc::User &, const string &) {}

  virtual void removeChatParticipant(const sdc::User &, const string &) {}

  virtual void appendMessageToChat(const sdc::ByteSeq &, const string &,
    const sdc::User &) {}

  virtual string echo(const string &) { return ""; }

  virtual ~ChatClientCallbackInd() {}
};

#endif
