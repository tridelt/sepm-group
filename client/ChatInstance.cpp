#include "ChatInstance.h"

//Qt
#include <QString>
#include <QTime>

//homebrew
#include "Logging.h"

//helper
#include "sdcHelper.h"

	ChatInstance::ChatInstance(sdc::StringSeq users, string chatID, sdc::ByteSeq key,
		function<void(sdc::ByteSeq, string)> sendCallback){
		
		this->users = users;
		this->chatID = chatID;
		this->key  = key;
		this->sendCallback = sendCallback;
		

		chatwin = new QDialog;
    	cw_ui.setupUi(chatwin);
    	chatwin->show();

    	//Connect Signals to Slots
		connect(cw_ui.btn_send, SIGNAL(clicked()), this, SLOT(sendMessage()));
	}

	void ChatInstance::sendMessage(){
		
		//TODO Exceptions

		INFO("Send message from Chat-Window to Chat-Manager via Callback-Object");

		QString message = cw_ui.txt_message->toPlainText();

		sendCallback(sdc::sdcHelper::bStringToByteSeq(message.toLocal8Bit().data(),
			message.length()), this->chatID);
	}

	void ChatInstance::addChatParticipant(sdc::User participant){
		cout << participant.ID << endl;
	}

	void ChatInstance::removeChatParticipant(sdc::User participant){
		cout << participant.ID << endl;
	}

	void ChatInstance::appendMessageToChat(sdc::ByteSeq message, sdc::User participant){

		INFO("Try to append Message to Chat-Window!");

		QTime time = QTime::currentTime();
		QString timeString = time.toString();

		QString text = cw_ui.txt_message->toPlainText();

		//Convert Byte-Array to String
		string msg = sdc::sdcHelper::getBinaryString(message);

		cout << participant.ID << endl;

		//Append Message to Chat-Window
		cw_ui.txt_chat->setText(cw_ui.txt_chat->text() + "[" + timeString + "] " +
			QString::fromStdString(participant.ID) + " " + QString::fromStdString(msg) + "\n");

		INFO("Message appended to Chat Window");

	}