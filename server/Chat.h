#ifndef CHAT_H
#define CHAT_H

#include "SecureDistributedChat.h"
//#include "UserManager.h"

using namespace std;
class Chat{
  public:
    Chat(string);
    string getName();
    void addUser(const sdc::User&);
    void rmUser(const sdc::User&);
    sdc::User getUser(const string&);
  private:
    string name;
    map<string,sdc::User> users;
    //UserManager *usermng;
    //TODO UserManager implementation
};

#endif

