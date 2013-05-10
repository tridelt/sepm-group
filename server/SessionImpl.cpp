#include "SessionImpl.h"
#include <IceUtil/IceUtil.h>
#include "Logging.h"

SessionImpl::SessionImpl(sdc::User u, DBPool *p) : loggedIn(true), user(u), db_pool(p) {
}

void SessionImpl::logout(const Ice::Current&) {
  INFO("<stub> logging out ", user.ID);
  this->loggedIn = false;
}


void SessionImpl::deleteUser(const sdc::User &other, const Ice::Current&) {
  INFO("<stub> ", user.ID, " deleting user ", other.ID);
}

string SessionImpl::initChat(const Ice::Current&) {
  INFO("<stub> initChat by ", user.ID);
  return "";
}

void SessionImpl::leaveChat(const string &chat, const Ice::Current&) {
  INFO("<stub> ", user.ID, " leaves chat ", chat);
}


void SessionImpl::invite(const sdc::User &other, const string &chat,
  const sdc::ByteSeq &, const Ice::Current&) {
  INFO("<stub> ", user.ID, " invites ", other.ID, " to ", chat);
}

void SessionImpl::sendMessage(const sdc::ByteSeq &, const string &chat,
  const Ice::Current&) {
  INFO("<stub> ", user.ID, " posts to ", chat);
}
