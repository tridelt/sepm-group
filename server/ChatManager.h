#ifndef CHATMANAGER_H
#define CHATMANAGER_H

#include "SecureDistributedChat.h"
using namespace std;

class ChatManager {
  public:
    String newChat();
    void rmChat(String);    
  private:
    map <String, Chat> chats;
    int i;
};

#endif
