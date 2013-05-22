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

	public:
		ChatInstance();
		ChatInstance(sdc::StringSeq, string, sdc::ByteSeq, function<void(sdc::ByteSeq, string)>);
		void addChatParticipant(sdc::User);
		void removeChatParticipant(sdc::User);
		void appendMessageToChat(sdc::ByteSeq, sdc::User);
		std::string id(void);
		~ChatInstance();

	private:
		int findUser(QString userID);
		void leaveChat();
		

	public slots:
		void sendMessage();
};

#endif