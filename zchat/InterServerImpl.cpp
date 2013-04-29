#include "InterServerImpl.h"

sdc::User InterServerImpl::retrieveUser(const string &name, const Ice::Current&) {
  sdc::User u;
  u.ID = name;
  return u;
}

void InterServerImpl::invite(const sdc::User&, const string&, const sdc::ByteSeq&, const Ice::Current&) {}
void InterServerImpl::sendMessage(const sdc::User&, const sdc::ByteSeq&, const string&, const Ice::Current&) {}
void InterServerImpl::leaveChat(const sdc::User&, const string&, const Ice::Current&) {}
void InterServerImpl::clientInitChat(const sdc::User&, const sdc::StringSeq&, const string&, const sdc::ByteSeq&, const Ice::Current&) {}
void InterServerImpl::clientAddChatParticipant(const sdc::User&, const sdc::User&, const string&, const Ice::Current&) {}
void InterServerImpl::clientRemoveChatParticipant(const sdc::User&, const sdc::User&, const string&, const Ice::Current&) {}
void InterServerImpl::clientAppendMessageToChat(const sdc::User&, const sdc::ByteSeq&, const string&, const sdc::User&, const Ice::Current&) {}
