#include "SessionImpl.h"
#include <IceUtil/IceUtil.h>
#include "DBPool.h"
#include "sdcHelper.h"
#include "SessionImpl.h"
#include "Logging.h"
#include "Chat.h"
#include "SessionManager.h"

using namespace std;
using namespace soci;

SessionImpl::SessionImpl(sdc::User u, shared_ptr<IceServer> srv, shared_ptr<ChatClientCallbackInd> cb) :
  loggedIn(true), user(u), server(srv), callback(cb) {
    server->session_mgr->addSession(shared_ptr<SessionImpl>(this));
}

sdc::User SessionImpl::retrieveUser(const string &id, const Ice::Current&) {
  INFO("<stub> retrieveUser ", id);
  return sdc::User();
}

void SessionImpl::logout(const Ice::Current&) {
  INFO("logging out ", user.ID);
  this->loggedIn = false;
  server->session_mgr->removeSession(user.ID, shared_ptr<SessionImpl>(this));
}

void SessionImpl::deleteUser(const sdc::User &u, const Ice::Current&) {
  //TODO: implement permission model so privileged users can delete others?
  // (for now only delete own account)
  if(u.ID != user.ID)
    throw sdc::UserHandlingException("deleting other users is not permitted");

  string name = u.ID;
  if(sdc::sdcHelper::isValidID(u.ID))
    name = sdc::sdcHelper::getNameFromID(u.ID);

  session sql(server->db_pool->getPool());

  try {
    sql << "DELETE FROM users WHERE id = :id", use(name);
  }
  catch(const soci_error &e) {
    // assuming nothing else can go wrong
    throw sdc::UserHandlingException("unknown ID");
  }

  // note: the user is not actually logged out, but they won't be able to store
  // data on the server or login again (we'd have to _reliably_ disconnect the
  // user otherwise, and Ice makes that slightly complicated)

  INFO(user.ID, " deleted account of ", u.ID);
}

string SessionImpl::initChat(const Ice::Current&) {
  INFO("initChat by ", user.ID);
  auto cp = server->chat_mgr->newChat();
  //TODO: prevent creation of new chat with exisiting name
  cp->addUser(user);
  return cp->getName();
}

void SessionImpl::leaveChat(const string &chat, const Ice::Current&) {
  INFO("leaveChat ", user.ID, " leaves chat ", chat);
  shared_ptr<Chat> cp;
  try {
    cp = server->chat_mgr->getChat(chat);
  } catch(...) {
    //TODO: forward to remote server if chat is not local
    //remote->leaveChat(user, chat);
    throw sdc::SessionException("chat " + chat + " does not exist");
  }
  chatBroadcastCallback(cp, server, removeChatParticipant(user, chat), clientRemoveChatParticipant(_u, user, chat));
  if(!cp->rmUser(user)) throw sdc::SessionException("you are not in chat " + chat);
}


void SessionImpl::invite(const sdc::User &other, const string &chat,
  const sdc::ByteSeq &pubkey, const Ice::Current&) {
  INFO(user.ID, " invites ", other.ID, " to ", chat);

  auto c = server->chat_mgr->getChat(chat);
  //TODO: relay to remote server if applicable
  userCallback(server, other, initChat(c->getUserList(), chat, pubkey));
  auto pk = pubkey;
}

void SessionImpl::sendMessage(const sdc::ByteSeq &msg, const string &chat,
  const Ice::Current&) {
  INFO(user.ID, " posts to ", chat, ": ", &msg);
  /*shared_ptr<Chat> cp;
  try {
    cp = server->chat_mgr->getChat(chat);
  } catch(...) {
    //TODO: forward to remote server if chat is not local
    //remote->sendMessage(user, msg, chat);
    throw sdc::MessageException("chat " + chat + " does not exist");
  }
  chatBroadcastCallback(cp, server, appendMessageToChat(msg, chat, user), clientAppendMessageToChat(_c, msg, chat, user));*/
}

void SessionImpl::saveLog(const string&, Ice::Long, const sdc::SecureContainer&, const Ice::Current&) {
  INFO("<stub> saveLog for ", user.ID);
  //TODO: make sure user exists (can't save logs if deleted)
}

sdc::Loglist SessionImpl::retrieveLoglist(const Ice::Current&) {
  INFO("<stub> retrieveLoglist for ", user.ID);
  return sdc::Loglist();
}

sdc::SecureContainer SessionImpl::retrieveLog(const string&, Ice::Long, const Ice::Current&) {
  INFO("<stub> retrieveLog for ", user.ID);
  auto container = sdc::SecureContainer();
  container.data = sdc::ByteSeq();
  container.signature = sdc::ByteSeq();
  return container;
}

void SessionImpl::saveContactList(const sdc::SecureContainer&, const Ice::Current&) {
  //TODO: make sure user exists (can't save contact list if deleted)
  INFO("<stub> saveContactList for ", user.ID);
}

sdc::SecureContainer SessionImpl::retrieveContactList(const Ice::Current&) {
  INFO("<stub> retrieveContactList for ", user.ID);
  auto container = sdc::SecureContainer();
  container.data = sdc::ByteSeq();
  container.signature = sdc::ByteSeq();
  return container;
}
