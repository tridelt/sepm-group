#include "Chat.h"
#include "SecureDistributedChat.h"

using namespace std;
Chat::Chat(string s) : name(s){
}

string Chat::getName(){
  return name;
}

void Chat::addUser(const sdc::User &u){
  users[u.ID]=u;
}

bool Chat::rmUser(const sdc::User &u){
  return users.erase(u.ID) == 1;
}

sdc::User Chat::getUser(const string &s){
  return users[s];
}

bool Chat::hasUser(const string &n) {
  try {
    users.at(n);
  } catch(...) {
    return false;
  }
  return true;
}

vector<sdc::User> Chat::getUsers() {
  vector<sdc::User> list;
  for(map<string, sdc::User>::iterator iter = users.begin();
      iter != users.end(); ++iter) {
    list.push_back(iter->second);
  }
  return list;
}

vector<string> Chat::getUserList() {
  vector<string> list;
  for(map<string, sdc::User>::iterator iter = users.begin();
      iter != users.end(); ++iter) {
    list.push_back(iter->first);
  }
  return list;
}
