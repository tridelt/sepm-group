#ifndef CHATMANAGER_H
#define CHATMANAGER_H

#include "SecureDistributedChat.h"
#include <boost/shared_ptr.hpp>
#include "Chat.h"

using namespace std;

class ChatManager {
  public:
    typedef map <string, shared_ptr<Chat>> chatlist;
    ChatManager();
    shared_ptr<Chat> newChat();
    shared_ptr<Chat> getChat(const string &name);
    chatlist *getChats();
    void rmChat(string);
  private:
    chatlist chats;
    int i;
};

#endif
