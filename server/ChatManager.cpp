#include "ChatManager.h"
#include "Chat.h"

using namespace std;
shared_ptr<Chat> ChatManager::newChat(){
	shared_ptr<Chat> cp;
  cp.reset(new Chat("Chat"+(i++)));
  chats[cp->getName()]=cp;
  return cp;
}
void ChatManager::rmChat(string){
	//TODO implement rmChat
}