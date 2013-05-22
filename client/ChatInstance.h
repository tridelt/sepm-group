#ifndef ChatInstance_H
#define ChatInstance_H

#include "SecureDistributedChat.h"

//std
#include <string>

//Qt
#include "QObject"
#include <QDialog>

//UI
#include "ui_ChatMessageWindow.h"

using namespace std;

class ChatInstance : public QObject{

	Q_OBJECT

	//List of Participants
	sdc::StringSeq users;
	string chatID;
	sdc::ByteSeq key;

	//UI
	QDialog *chatwin;
	Ui_ChatMessageWindow cw_ui;

	//Callback to ChatManager for send-Message
	function<void(sdc::ByteSeq, string)> sendCallback;
	//Callback to ChatManager for leave-Chat
	function<void(string)> leaveChatCallback;
	//Callback to ChatManager for invite
	//function<void(string)> inviteCallback;

	//TODO Callback for Invite-User

	public:
		ChatInstance();
		ChatInstance(sdc::StringSeq, string, sdc::ByteSeq, function<void(sdc::ByteSeq, string)>, function<void(string)>);
		void addChatParticipant(sdc::User);
		void removeChatParticipant(sdc::User);
		void appendMessageToChat(sdc::ByteSeq, sdc::User);
		std::string id(void);
		~ChatInstance();

		//TODO invite(User participant, string chatID, ByteSeq sessionKey)

	private:
		int findUser(QString userID);
		

	public slots:
		void sendMessage();
		void invite();
		void leaveChat();
};

#endif