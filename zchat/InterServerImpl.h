#include "SecureDistributedChat.h"
#include <string>

using namespace std;

class InterServerImpl : public virtual sdc::InterServerI {
  /** return information about user */
  sdc::User retrieveUser(const string&, const Ice::Current&);


  void invite(const sdc::User&, const string&, const sdc::ByteSeq&, const Ice::Current&);
  void sendMessage(const sdc::User&, const sdc::ByteSeq&, const string&, const Ice::Current&);
  void leaveChat(const sdc::User&, const string&, const Ice::Current&);
  void clientInitChat(const sdc::User&, const sdc::StringSeq&, const string&, const sdc::ByteSeq&, const Ice::Current&);
  void clientAddChatParticipant(const sdc::User&, const sdc::User&, const string&, const Ice::Current&);
  void clientRemoveChatParticipant(const sdc::User&, const sdc::User&, const string&, const Ice::Current&);
  void clientAppendMessageToChat(const sdc::User&, const sdc::ByteSeq&, const string&, const sdc::User&, const Ice::Current&);
};
