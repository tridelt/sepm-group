#ifndef CHATMANAGER_H
#define CHATMANAGER_H

#include "SecureDistributedChat.h"
#include "Chat.h"
using namespace std;

class ChatManager {
  public:
    shared_ptr<Chat> newChat();
    void rmChat(string);
    //TODO getChat 
  private:
    map <string, shared_ptr<Chat>> chats;
    int i;
};

#endif
