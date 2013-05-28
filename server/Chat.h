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

/**
 * send a message to all users (local callback to local users, or InterServer
 * message for remote ones
 */
#define chatBroadcastCallback(chat, srv, local, remote) {\
  vector<sdc::User> ul = chat->getUsers();\
  for(vector<sdc::User>::iterator iter = ul.begin(); iter != ul.end(); ++iter) {\
    sdc::User _u = *iter;\
    if(srv->isLocal(_u.ID)) {\
      userCallback(srv, _u, local);\
    }\
    else {\
      /* TODO: InterServer */\
    }\
  }\
}

#endif
