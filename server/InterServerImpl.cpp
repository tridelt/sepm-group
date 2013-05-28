#include "InterServerImpl.h"
#include "Logging.h"
#include "DBPool.h"
#include "sdcHelper.h"
#include "SessionImpl.h"

using namespace soci;

InterServerImpl::InterServerImpl(IceServerI *srv) : server(srv) {
}

User InterServerImpl::retrieveUser(const string &id, const Ice::Current&) {
  INFO("<retrieveUser ", id);

  string name = id;
  if(sdc::sdcHelper::isValidID(id)) {
    name = sdc::sdcHelper::getNameFromID(id);
  }
  else WARN("malformed ID ", id);
  
  session sql(server->getDBPool()->getPool());
  boost::optional<string> pubkey;
  sql << "SELECT pubkey FROM users WHERE id = :id", into(pubkey), use(name);

  if(pubkey.is_initialized()) {
    WARN("retrieveUser for unknown user ", id);
    throw sdc::InterServerException("user " + id + " is not known on this server");
  }

  sdc::User u;
  u.ID = id;
  u.publicKey = sdc::ByteSeq(pubkey.get().begin(), pubkey.get().end());
  return u;
}

void InterServerImpl::invite(const User &participant, const string &chat, const ByteSeq &key, const Ice::Current&) {
  INFO("invite ", participant.ID, " to ", chat);

  auto sl = server->getSessions()->getSessions(participant.ID);
  
  if(sl.first == sl.second) {
    throw sdc::InterServerException("user " + participant.ID + " is not online");
  }
  
  for(auto iter = sl.first; iter != sl.second; ++iter) {
    //FIXME: we don't know who's in the chat 
    iter->second->getCallback()->initChat(sdc::StringSeq(), chat, key);
  }
}

void InterServerImpl::sendMessage(const User &sender, const ByteSeq &msg, const string &chat, const Ice::Current&) {
  INFO("sendMessage from ", sender.ID, " to ", chat);

  shared_ptr<Chat> cp;
  try {
    cp = server->getChats()->getChat(chat);
  } catch(...) {
    throw sdc::InterServerException("chat " + chat + " does not exist here");
  }

  auto ml = cp->getUsers();
  for(const sdc::User &u : ml) {
    try {
      INFO("relaying message to user ", u.ID);
      server->getISManager()->getServerForID(u.ID)->clientAppendMessageToChat(u, msg, chat, sender);
    } catch(...) {
      // ignore any errors on our end, these don't concern the remote server
    }
  }
}

void InterServerImpl::leaveChat(const User &participant, const string &chat, const Ice::Current &) {
  INFO("leaveChat ", participant.ID, " leaving ", chat);

  shared_ptr<Chat> cp;
  try {
    cp = server->getChats()->getChat(chat);
  } catch(...) {
    throw sdc::InterServerException("chat " + chat + " does not exist here");
  }
  
  if(!cp->rmUser(participant)) throw sdc::InterServerException("user " + participant.ID + " is not in chat " + chat);

  auto ml = cp->getUsers();
  for(const sdc::User &u : ml) {
    try {
      server->getISManager()->getServerForID(u.ID)->clientRemoveChatParticipant(u, participant, chat);
    } catch(...) {
      // ignore any errors on our end, these don't concern the remote server
    }
  }
}

void InterServerImpl::clientInitChat(const User &recipient, const StringSeq&, const string &chat, const ByteSeq&, const Ice::Current&) {
  INFO("<stub> clientInitChat notify ", recipient.ID, " of ", chat);
}

void InterServerImpl::clientAddChatParticipant(const User &recipient, const User &other, const string &chat, const Ice::Current&) {
  INFO("<stub> clientAddChatParticipant notify ", recipient.ID, " that ", other.ID, " joined ", chat);
}

void InterServerImpl::clientRemoveChatParticipant(const User &recipient, const User &other, const string &chat, const Ice::Current&) {
  INFO("<stub> clientRemoveChatParticipant notify ", recipient.ID, " that ", other.ID, " left ", chat);


}

void InterServerImpl::clientAppendMessageToChat(const User &recipient, const ByteSeq &msg, const string &chat, const User &sender, const Ice::Current&) {
  INFO("clientAppendMessageToChat notify ", recipient.ID, " that ", sender.ID, " sent a message to ", chat);

  auto sl = server->getSessions()->getSessions(recipient.ID);

  if(sl.first == sl.second) {
    throw sdc::InterServerException("user " + recipient.ID + " is not online");
  }

  for(auto iter = sl.first; iter != sl.second; ++iter) {
    auto cb = iter->second->getCallback();
    //FIXME: callback hangs everything - client's fault?
    //iter->second->getCallback()->appendMessageToChat(msg, chat, sender);
  }
  auto msg_ = msg;
}

