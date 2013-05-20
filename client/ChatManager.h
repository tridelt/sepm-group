#ifndef CHATMANAGER_H
#define CHATMANAGER_H

//std
#include <stdexcept>
#include <string>

//Qt
#include <QString>
#include <QMultiHash>

//Ice
#include "SecureDistributedChat.h"
#include <Ice/Ice.h>
#include <IceSSL/IceSSL.h>

//homebrew
#include "ChatInstance.h"

namespace cm{

	//Exceptions
	class ServerUnavailableException : public std::runtime_error {
			public:
				ServerUnavailableException() : std::runtime_error("Could not establish connection to server!") { }
	};
	class FileNotFoundException : public std::runtime_error {
			public:
				FileNotFoundException() : std::runtime_error("could not find file!") { }
	};
	class AlreadyRegisteredException : public std::runtime_error {
			public:
				AlreadyRegisteredException() : std::runtime_error("Username already registerd!") { }
	};
	class CommunicationException : public std::runtime_error {
			public:
				CommunicationException() : std::runtime_error("Communication error!") { }
	};
	class NotLoggedInException : public std::runtime_error {
			public:
				NotLoggedInException() : std::runtime_error("Please login first!") { }
	};
	class InvalidChatIDException : public std::runtime_error {
			public:
				InvalidChatIDException() : std::runtime_error("Unknown or invalid ChatID!") { }
	};

	//TODO: Singleton
	class ChatManager{

		// list of all ChatInstances
		QMultiHash<QString, ChatInstance*> *chats;

		// connection information
		std::string host;
		std::string cert_path;
		int port;
		
		// ice
		Ice::PropertiesPtr props;
		Ice::CommunicatorPtr ic;
		Ice::InitializationData id;

		Ice::ObjectPrx base;
		sdc::SessionIPrx session;
		

		public:
			ChatManager(std::string hostname, int port, std::string cert) throw(ServerUnavailableException, FileNotFoundException);
			void registerUser(sdc::User user, QString pwd) throw (AlreadyRegisteredException, CommunicationException);
			bool isOnline(void);
			bool isLoggedin(void);
			void login(sdc::User user, std::string pwd) throw (CommunicationException);
			void logout(void) throw (CommunicationException, NotLoggedInException);
			//to create a new one
			QString initChat() throw (CommunicationException, NotLoggedInException, InvalidChatIDException);
			
			//ChatClientCallbackI

			//to load existing chats
			void initChat(const sdc::StringSeq&, const std::string&, const sdc::ByteSeq&, const Ice::Current&);
			void addChatParticipant(const sdc::User&, const std::string&, const Ice::Current&) {}
			void removeChatParticipant(const sdc::User&, const std::string&, const Ice::Current&){};
			void appendMessageToChat(const sdc::ByteSeq&, const std::string&, const sdc::User&, const Ice::Current&);
			void sendMessage(const sdc::ByteSeq&, const std::string&) throw (CommunicationException, NotLoggedInException, InvalidChatIDException);

			std::string echo(const std::string &s, const Ice::Current&) { return s; }
			~ChatManager();
	};
}

#endif //CHATMANAGER_H