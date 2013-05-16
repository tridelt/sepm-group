#ifndef CHATMANAGER_H
#define CHATMANAGER_H

#include "SecureDistributedChat.h"
using namespace std;

class ChatManager {
  public:
    ChatManager();
    addChat(Chat);
    
  private:
    map <String, Chat> chats;
};

#endif
