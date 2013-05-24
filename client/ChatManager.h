#ifndef CHATMANAGER_H
#define CHATMANAGER_H

//std
#include <stdexcept>
#include <string>
#include <memory>

//Qt
#include <QString>
#include <QList>

//Ice
#include "SecureDistributedChat.h"
#include <Ice/Ice.h>
#include <IceSSL/IceSSL.h>

//helper
 #include "Security.h"

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
	class SignatureException : public std::runtime_error {
			public:
				SignatureException() : std::runtime_error("Seems to found wrong signation!") { }
	};

	//TODO: Singleton
	class ChatManager : public virtual sdc::ChatClientCallbackI{

		// list of all ChatInstances
		QList<std::shared_ptr<ChatInstance>> chats;

		sdc::Security sec;

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

		// security
		int keysize;

		//logged in User
		sdc::User loggedInUser;

		//contacts
		sdc::ContactList contacts;

		public:
			sdc::ByteSeq privateKey;


			ChatManager(std::string hostname, int port, std::string cert) throw(ServerUnavailableException, FileNotFoundException);
			void registerUser(sdc::User user, QString pwd) throw (AlreadyRegisteredException, CommunicationException);
			bool isOnline(void);
			bool isLoggedin(void);
			void login(sdc::User user, QString pwd) throw (CommunicationException);

			//ChatClientCallbackI

			//to load existing chats
			//TODO: add throw
			void initChat(const sdc::StringSeq&, const std::string&, const sdc::ByteSeq&, const Ice::Current&);
			void addChatParticipant(const sdc::User&, const std::string&, const Ice::Current&);
			void removeChatParticipant(const sdc::User&, const std::string&, const Ice::Current&);
			void appendMessageToChat(const sdc::ByteSeq&, const std::string&, const sdc::User&, const Ice::Current&);

			//SessionI
			void logout(void) throw (CommunicationException, NotLoggedInException);
			QString initChat(void) throw (CommunicationException, NotLoggedInException, InvalidChatIDException);
			void leaveChat(const std::string&);
			//void invite(const sdc::User&, const string&, const sdc::ByteSeq&);
			void sendMessage(const sdc::ByteSeq&, const std::string&) throw (CommunicationException, NotLoggedInException);



			std::string echo(const std::string &s, const Ice::Current&) { return s; }

			//gui helpers
			QList<QString>* getContacts(void);
			~ChatManager();
		private:
			std::shared_ptr<ChatInstance> findChat(std::string chatID) throw (InvalidChatIDException);
			//
			void retrieveContactList() throw(CommunicationException, NotLoggedInException);
			void saveContactList() throw(CommunicationException, NotLoggedInException);
	};
}

#endif //CHATMANAGER_H
