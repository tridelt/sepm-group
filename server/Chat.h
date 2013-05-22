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
    /**
     * @return false if user was not in chat, true otherwise
     */
    bool rmUser(const sdc::User&);
    sdc::User getUser(const string&);
    vector<sdc::User> getUsers();
    vector<string> getUserList();
  private:
    string name;
    map<string,sdc::User> users;
    //UserManager *usermng;
    //TODO UserManager implementation
};

#endif
