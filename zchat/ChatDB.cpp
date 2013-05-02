#include "ChatDB.h"
#include "Security.h"
#include "sdcHelper.h"

ChatDB* ChatDB::instance;
boost::mutex ChatDB::mutex;


ChatDB::ChatDB() {
  sdc::Security sec;
  sec.genRSAKey(publicKey, privateKey);
}

ChatDB* ChatDB::i() {
  if(instance == NULL) {
    boost::lock_guard<boost::mutex> lock(mutex);
    if(instance == NULL)
      instance = new ChatDB();
  }
  return instance;
}

set<string> ChatDB::usersForChat(const string &chat) {
  boost::lock_guard<boost::mutex> lock(mutex);

  if(chatUsers.count(chat) > 0)
    return set<string>(chatUsers[chat]);
  else
    return set<string>();
}

sdc::InterServerI* ChatDB::serverForUser(const string &user) {
  boost::lock_guard<boost::mutex> lock(mutex);

  if(userServers.count(user) == 0)
    return &localServer;

  return userServers[user];
}

sdc::InterServerI* ChatDB::serverForChat(const string &chat) {
  boost::lock_guard<boost::mutex> lock(mutex);

  if(chatServers.count(chat) == 0)
    return &localServer;

  return chatServers[chat];
}


sdc::User ChatDB::userForString(const string &user) {
  boost::lock_guard<boost::mutex> lock(mutex);

  if(users.count(user) == 0) {
    if(sdc::sdcHelper::isValidID(user)) {
      auto serverStr = sdc::sdcHelper::getServerFromID(user);
      auto server = serverForString(serverStr);

      users[user] = server->retrieveUser(user);

      cout << "got user: " << users[user].ID << " for " << user << endl;
      string dump_key(users[user].publicKey.begin(), users[user].publicKey.end());
      cout << dump_key << endl;

      sdc::Security sec;
      sec.savePubKey(users[user].publicKey, "pubkey2");
    } else {
      // local user
      sdc::User u;
      u.ID = user;
      u.publicKey = publicKey;
      users[user] = u;
    }
  }

  return users[user];
}

string ChatDB::decryptMsgForChat(const string &chat, const sdc::ByteSeq &msg) {
  boost::lock_guard<boost::mutex> lock(mutex);

  if(chatKeys.count(chat) == 0) {
    cout << "can't find channel " << chat << ", skipping decryption" << endl;
    return string(msg.begin(), msg.end());
  }

  sdc::Security sec;
  auto plain = sec.decryptAES(chatKeys[chat], msg);

  return string(plain.begin(), plain.end());
}


sdc::ByteSeq ChatDB::encryptMsgForChat(const string &chat, const string &msg) {
  boost::lock_guard<boost::mutex> lock(mutex);

  if(chatKeys.count(chat) == 0) {
    cout << "can't find channel " << chat << ", skipping encryption" << endl;
    return sdc::ByteSeq(msg.begin(), msg.end());
  }

  sdc::Security sec;
  sdc::ByteSeq plain(msg.begin(), msg.end());
  auto cryptText = sec.encryptAES(chatKeys[chat], plain);

  return sdc::ByteSeq(cryptText);
}

void ChatDB::setKeyForChat(const string &chat, const sdc::ByteSeq &key) {
  boost::lock_guard<boost::mutex> lock(mutex);

  chatKeys[chat] = sdc::ByteSeq(key);
}


sdc::ByteSeq ChatDB::getKeyForChat(const string &chat) {
  boost::lock_guard<boost::mutex> lock(mutex);

  if(chatKeys.count(chat) == 0) {
    sdc::Security sec;
    chatKeys[chat] = sec.genAESKey(256);
  }

  return chatKeys[chat];
}

void ChatDB::addUserToChat(const string &chat, const string &user) {
  boost::lock_guard<boost::mutex> lock(mutex);

  if(chatUsers.count(chat) == 0)
    chatUsers[chat] = set<string>();

  chatUsers[chat].insert(user);
}

void ChatDB::removeUserFromChat(const string &chat, const string &user) {
  boost::lock_guard<boost::mutex> lock(mutex);

  chatUsers[chat].erase(user);
}


sdc::InterServerI* ChatDB::serverForString(const string &server) {
  if(servers.count(server) == 0) {
    cout << "create new IceClient for " << server << endl;
    servers[server] = new IceClient(server);
  }

  return servers[server]->getServer();
}
