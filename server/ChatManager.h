#ifndef CHATMANAGER_H
#define CHATMANAGER_H

#include "SecureDistributedChat.h"
#include "Chat.h"
using namespace std;

class ChatManager {
  public:
    string newChat();
    void rmChat(string);    
  private:
    map <string, shared_ptr<Chat>> chats;
    int i;
};

#endif
