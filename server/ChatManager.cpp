#include "ChatManager.h"
#include "Chat.h"

using namespace std;
ChatManager::addChat(Chat c){
  chats["Chat"+i] = c;
  c.setName("Chat"+i);
  i++;
}
