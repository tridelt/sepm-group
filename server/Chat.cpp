#include "Chat.h";

using namespace std;
String Chat::getName(){
  return name;
}

void Chat::setName(const String &s){
  name=s;
}

void Chat::addUser(const User &u){
  users[u.ID]=u;
}

void Chat::rmUser(const User &u){
  users.erase(u.ID);
}

User Chat::getUser(const String &s){
  return users[s];
}