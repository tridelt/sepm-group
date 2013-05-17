#ifndef CHATMANAGER_H
#define CHATMANAGER_H

#include "SecureDistributedChat.h"
#include <boost/shared_ptr.hpp>
#include "Chat.h"

using namespace std;

class ChatManager {
  public:
    ChatManager();
    shared_ptr<Chat> newChat();
    void rmChat(string);
    //TODO getChat
  private:
    map <string, shared_ptr<Chat>> chats;
    int i;
};

#endif
