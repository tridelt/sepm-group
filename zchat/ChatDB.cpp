#include "ChatDB.h"
#include "Security.h"

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

  return userServers[user];
}

sdc::User ChatDB::userForString(const string &user) {
  boost::lock_guard<boost::mutex> lock(mutex);

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
