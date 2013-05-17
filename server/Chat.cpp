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
