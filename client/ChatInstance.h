#include "SecureDistributedChat.h"
#include <string>


using namespace std;

class ChatInstance{

	sdc::StringSeq& users;
	string& chatID;
	sdc::ByteSeq& key;

	public:
		ChatInstance();
		ChatInstance(sdc::StringSeq, string, sdc::ByteSeq);
		void sendMessage();
		void addChatParticipant(sdc::User);
		void removeChatParticipant(sdc::User);
		void appendMessageToChat(sdc::ByteSeq, sdc::User);
		//void appendMessagetoChat();
};