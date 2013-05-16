#ifndef CHAT_H
#define CHAT_H

#include "SecureDistributedChat.h"
#include "UserManager.h"

using namespace std;
class Chat{
  public:
  private:
    String name;
    map<String,sdc::User> users;
    UserManager *usermng;

};