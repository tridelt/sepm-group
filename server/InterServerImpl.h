#ifndef INTER_SERVER_IMPL_H
#define INTER_SERVER_IMPL_H

#include "SecureDistributedChat.h"
#include "IceServerI.h"

using namespace std;
using namespace sdc;

class InterServerImpl : public virtual sdc::InterServerI {
public:
  InterServerImpl(IceServerI *srv);
  User retrieveUser(const string &id, const Ice::Current&);
  void invite(const User &participant, const string &chat, const ByteSeq &key, const Ice::Current&);
  void sendMessage(const User &sender, const ByteSeq &msg, const string &chat, const Ice::Current&);
  void leaveChat(const User&, const string&, const Ice::Current&);
  void clientInitChat(const User&, const StringSeq&, const string&, const ByteSeq&, const Ice::Current&);
  void clientAddChatParticipant(const User&, const User&, const string&, const Ice::Current&);
  void clientRemoveChatParticipant(const User&, const User&, const string&, const Ice::Current&);
  void clientAppendMessageToChat(const User&, const ByteSeq&, const string&, const User&, const Ice::Current&);
private:
  IceServerI *server;
};

#endif
