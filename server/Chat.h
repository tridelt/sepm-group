#ifndef CHAT_H
#define CHAT_H

#include "SecureDistributedChat.h"
#include "UserManager.h"

using namespace std;
class Chat{
  public:
    String getName();
    void setName();
    void addUser(const &User);
    void rmUser(const &User);
    User getUser(const &String);
  private:
    String name;
    map<String,sdc::User> users;
    UserManager *usermng;
};