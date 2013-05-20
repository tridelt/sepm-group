#include "ChatInstance.h"




	ChatInstance::ChatInstance(sdc::StringSeq users, string chatID, sdc::ByteSeq key) : users(users), chatID(chatID), key(key){
		/*
		this->users = users;
		this->chatID = chatID;
		this->key  = key;
		*/
	}

	void ChatInstance::sendMessage(){
		//TODO Nachricht an ChatManager weiterleiten
	}

	void ChatInstance::addChatParticipant(sdc::User participant){
		cout << participant.ID << endl;
	}

	void ChatInstance::removeChatParticipant(sdc::User participant){
		cout << participant.ID << endl;
	}

	void ChatInstance::appendMessageToChat(sdc::ByteSeq message, sdc::User participant){

		sdc::ByteSeq msg = message;
		cout << participant.ID << endl;
	}