#include "InterServerImpl.h"
#include "ChatDB.h"
#include "SocketHandler.h"
#include "Security.h"

/** return information about user to remote server */
sdc::User InterServerImpl::retrieveUser(const string &name, const Ice::Current&) {
  sdc::User u;
  u.ID = name;
  u.publicKey = ChatDB::i()->publicKey;
  return u;
}


/** forward invite to chat from remote server to 'local' user (chat is on remote
    server) */
void InterServerImpl::invite(const sdc::User &recipient, const string &chat,
  const sdc::ByteSeq &encryptedKey, const Ice::Current&) {
  sdc::Security sec;
  auto decryptedKey = sec.decryptRSA(ChatDB::i()->privateKey, encryptedKey);
  ChatDB::i()->setKeyForChat(chat, decryptedKey);
  ChatDB::i()->addUserToChat(chat, recipient.ID);
}


/** receive message from remote user for chat on local server */
void InterServerImpl::sendMessage(const sdc::User &sender, const sdc::ByteSeq &msg,
  const string &chat, const Ice::Current&) {
  for(auto userStr : ChatDB::i()->usersForChat(chat)) {
    auto server = ChatDB::i()->serverForUser(userStr);
    auto user = ChatDB::i()->userForString(userStr);
    server->clientAppendMessageToChat(user, msg, chat, sender);
  }
}

/** remote user leaves local chat */
void InterServerImpl::leaveChat(const sdc::User &leaving_user, const string &chat,
  const Ice::Current&) {
  ChatDB::i()->removeUserFromChat(chat, leaving_user.ID);

  for(auto userStr : ChatDB::i()->usersForChat(chat)) {
    auto server = ChatDB::i()->serverForUser(userStr);
    auto user = ChatDB::i()->userForString(userStr);
    server->clientRemoveChatParticipant(user, leaving_user, chat);
  }

}

void InterServerImpl::clientInitChat(const sdc::User&, const sdc::StringSeq&, const string&, const sdc::ByteSeq&, const Ice::Current&) {}


/** tell local client somebody joined remote chat */
void InterServerImpl::clientAddChatParticipant(const sdc::User &recipient,
  const sdc::User &joining_user, const string &chat, const Ice::Current&) {
  SocketHandler::i()->send_chan_event(chat, recipient.ID, "join", joining_user.ID);
}


/** tell local client somebody left remote chat */
void InterServerImpl::clientRemoveChatParticipant(const sdc::User &recipient,
  const sdc::User &leaving_user, const string &chat, const Ice::Current&) {
  SocketHandler::i()->send_chan_event(chat, recipient.ID, "quit", leaving_user.ID);
}


/** receive message from remote chat for local user */
void InterServerImpl::clientAppendMessageToChat(const sdc::User &recipient,
    const sdc::ByteSeq &msg, const string &chat, const sdc::User &sender,
    const Ice::Current&) {

  string msg_str = ChatDB::i()->decryptMsgForChat(chat, msg);
  SocketHandler::i()->send_msg(chat, recipient.ID, sender.ID, msg_str);
}
