#include "SessionImpl.h"
#include <IceUtil/IceUtil.h>

SessionImpl::SessionImpl(sdc::User u, DBPool *p) : loggedIn(true), user(u), db_pool(p) {
}

void SessionImpl::logout(const Ice::Current&) {
  this->loggedIn = false;
}


void SessionImpl::deleteUser(const sdc::User&, const Ice::Current&) {
}

string SessionImpl::initChat(const Ice::Current&) {
  return "";
}

void SessionImpl::leaveChat(const string &, const Ice::Current&) {
}


void SessionImpl::invite(const sdc::User &, const string &,
  const sdc::ByteSeq &, const Ice::Current&) {
}

void SessionImpl::sendMessage(const sdc::ByteSeq &, const string &,
  const Ice::Current&) {
}
