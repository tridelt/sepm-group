#include "ChatManager.h"
#include "Chat.h"

using namespace std;
ChatManager::addChat(Chat c){
ChatManager::addChat(const Chat &c){
  chats["Chat"+i] = c;
  c.setName("Chat"+i);
  i++;
}
