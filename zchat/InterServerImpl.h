#ifndef INTERSERVER_IMPL_H
#define INTERSERVER_IMPL_H

#include "SecureDistributedChat.h"
#include <string>

using namespace std;

class InterServerImpl : public virtual sdc::InterServerI {
  /** return information about user to remote server */
  sdc::User retrieveUser(const string &user, const Ice::Current&);

  /** forward invite to chat from remote server to 'local' user (chat is on remote
      server) */
  void invite(const sdc::User &recipient, const string &chat,
    const sdc::ByteSeq &encryptedKey, const Ice::Current&);

  /** receive message from remote user for chat on local server */
  void sendMessage(const sdc::User &sender, const sdc::ByteSeq &msg,
    const string &chat, const Ice::Current&);

  /** remote user leaves local chat */
  void leaveChat(const sdc::User &user, const string &chat, const Ice::Current&);

  /** tell local client about remote chat */
  void clientInitChat(const sdc::User&, const sdc::StringSeq&, const string&, const sdc::ByteSeq&, const Ice::Current&);

  /** tell local client somebody joined remote chat */
  void clientAddChatParticipant(const sdc::User&, const sdc::User&, const string&, const Ice::Current&);

  /** tell local client somebody left remote chat */
  void clientRemoveChatParticipant(const sdc::User&, const sdc::User&, const string&, const Ice::Current&);

  /** receive message from remote chat for local user */
  void clientAppendMessageToChat(const sdc::User &recipient,
    const sdc::ByteSeq &msg, const string &chat, const sdc::User &sender,
    const Ice::Current&);
};

#endif
