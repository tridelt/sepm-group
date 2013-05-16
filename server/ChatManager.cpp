#include "ChatManager.h"
#include "Chat.h"

using namespace std;
string ChatManager::newChat(){
  Chat *c= new Chat;
  c->setName("Chat"+(i++));
  chats[c->getName()].reset(c);
  return c->getName();
}
void ChatManager::rmChat(string){
	//TODO implement rmChat
}