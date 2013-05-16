#include "ChatManager.h"
#include "Chat.h"

using namespace std;
<<<<<<< Updated upstream
ChatManager::addChat(Chat c){
  chats
=======
ChatManager::addChat(const Chat &c){
  chats["Chat"+i] = c;
  c.setName("Chat"+i);
  i++;
>>>>>>> Stashed changes
}
