#include "SecureDistributedChat.h"
#include <string>

using namespace std;

class InterServerImpl : public virtual sdc::InterServerI {
  /** return information about user to remote server */
  sdc::User retrieveUser(const string&, const Ice::Current&);

  /** forward invite to chat from remote server to 'local' user */
  void invite(const sdc::User&, const string&, const sdc::ByteSeq&, const Ice::Current&);

  /** receive message from remote user for chat on local server */
  void sendMessage(const sdc::User&, const sdc::ByteSeq&, const string&, const Ice::Current&);

  /** remote user leaves local chat */
  void leaveChat(const sdc::User&, const string&, const Ice::Current&);

  /** tell local client about remote chat */
  void clientInitChat(const sdc::User&, const sdc::StringSeq&, const string&, const sdc::ByteSeq&, const Ice::Current&);

  /** tell local client somebody joined remote chat */
  void clientAddChatParticipant(const sdc::User&, const sdc::User&, const string&, const Ice::Current&);

  /** tell local client somebody left remote chat */
  void clientRemoveChatParticipant(const sdc::User&, const sdc::User&, const string&, const Ice::Current&);

  /** receive message from remote chat for local user */
  void clientAppendMessageToChat(const sdc::User&, const sdc::ByteSeq&, const string&, const sdc::User&, const Ice::Current&);
};
