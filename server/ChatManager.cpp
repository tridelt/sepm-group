#include "ChatManager.h"
#include "Chat.h"

using namespace std;
String ChatManager::newChat(){
  Chat c;
  c.setName("Chat"+(i++));
  chats[c.getName()] = c;
  return c.getName();
}

void ChatManager::rmChat(){}
