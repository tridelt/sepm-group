
#include "ChatManager.h"

//boost
#include <boost/filesystem.hpp>

//ice
#include <IceUtil/IceUtil.h>

//homebrew
#include "Logging.h"

namespace cm{

	ChatManager::ChatManager(std::string host, int port, std::string cert_path)
		throw (ServerUnavailableException, FileNotFoundException):
		host(host), cert_path(cert_path), port(port), session(NULL),keysize(512) {

		INFO("try to establish connection");

		//try to find file
		if (!boost::filesystem::exists(cert_path)){
	    	ERROR("cert not found! " + cert_path);

	    	throw(FileNotFoundException());
		}

		///init ice

		try {
			props = Ice::createProperties();

		    // enable the SSL plugin for secure connections
		    props->setProperty("Ice.Plugin.IceSSL", "IceSSL:createIceSSL");

		    props->setProperty("IceSSL.CertAuthFile", cert_path);
		    // necessary so invalid connections will time out
		    props->setProperty("Ice.Override.ConnectTimeout", "1000");

		    // Initialize a communicator with these properties.
		    id.properties = props;
		    ic = Ice::initialize(id);

		    base = ic->stringToProxy("Authentication:ssl -h " + host +
		    	" -p " + boost::lexical_cast<string>(port));


		    // catch all manners of error cases
		  } catch (const Ice::ConnectTimeoutException& e) {
			ERROR("timeout while establishing connection - check server and port");

			throw(ServerUnavailableException());
		  } catch (const Ice::PluginInitializationException& e) {
			ERROR("failed to initialize SSL plugin - are you using the correct certificate?");

			//FIXME: find more suitable exception
			throw(ServerUnavailableException());
		  } catch (const Ice::EndpointParseException& e) {
			ERROR("Failed to create endpoint, check server and port: "
				+ host + "/"
				+ boost::lexical_cast<string>(port));

			throw(ServerUnavailableException());
		  } catch (const Ice::DNSException& e) {
			ERROR("error resolving hostname");

			throw(ServerUnavailableException());
		  }catch (const Ice::Exception& e) {
			ERROR(e.ice_name());

			throw(ServerUnavailableException());
		  }
	}

	/**
	 * registerUser
	 *
	 * register new chat user
	 */

	void ChatManager::registerUser(sdc::User user, QString pwd) throw
		(AlreadyRegisteredException, CommunicationException){
		try{
			sdc::AuthenticationIPrx auth = sdc::AuthenticationIPrx::checkedCast(base);

			auth->registerUser(user, pwd.toStdString());
		} catch(const sdc::AuthenticationException& e){
			ERROR(e.ice_name());
			throw(AlreadyRegisteredException());
		}
	}

	/**
	 * isOnline
	 *
	 * Helperfunction to test connectivity
	 *
	 * @return true if connected, otherwise false
	 */

	bool ChatManager::isOnline(void){
		try{
			base->ice_ping();
		} catch (const Ice::Exception& e) {
			return false;
		}

		return true;
	}

	/**
	 * isLoggedin
	 *
	 * Helperfunction to test authentication status
	 *
	 * @returns true if a chatuser is loggedin, otherwise false.
	 */

	bool ChatManager::isLoggedin(void){
		try{
			session->ice_ping();
		} catch (const Ice::Exception& e) {
			return false;
		}

		return true;
	}


	/**
	 * login
	 *
	 * Authentication method for Chat. Requirement for most chat functions.
	 *
	 * @param user user-struct
	 * @param pwd password for authentication purposes
	 */

	void ChatManager::login(sdc::User user, QString pwd) throw (CommunicationException){
		INFO("login " + user.ID);
		try{
			sdc::AuthenticationIPrx auth = sdc::AuthenticationIPrx::checkedCast(base);

			Ice::Identity ident;
			Ice::ObjectAdapterPtr adapter = ic->createObjectAdapter("");
			ident.name = IceUtil::generateUUID();
			ident.category = "";

			adapter->add(this, ident);
			adapter->activate();
			base->ice_getConnection()->setAdapter(adapter);

			session = auth->login(user, pwd.toStdString(), ident);

			//TODO: retrieveContactList

			retrieveContactList();
		} catch(const sdc::AuthenticationException& e){
			ERROR(e.ice_name());

		}

		loggedInUser = user;
	}

	/**
	 * initChat
	 *
	 * create new Chat
	 *
	 * @return chatID
	 */

	QString ChatManager::initChat() throw (CommunicationException, NotLoggedInException, InvalidChatIDException){
		INFO("initnew Chat");

		std::string chatID;

		//test if currently logged in
		if(!isLoggedin())
			throw(NotLoggedInException());

		//create new Chat
		try{
			chatID = session->initChat();
		} catch(sdc::SessionException& e){
			ERROR(e.ice_name());
			throw(CommunicationException());
		}


		//id validation
		if(chatID == "")
			throw(InvalidChatIDException());

		//Chat participants
		sdc::StringSeq users;

		//add current user
		//users.push_back(*loggedInUser);

		//generate session-key
		sdc::ByteSeq key = sec.genAESKey(keysize);

		INFO("DEBUG: new chat instance ID: " + chatID);

		auto ci = std::make_shared<ChatInstance>(users, chatID, key,
			boost::bind(&ChatManager::sendMessage, this, _1, _2),
			boost::bind(&ChatManager::leaveChat, this, _1));

		INFO("DEBUG: add participant");

		//add logged in User to the Chat-Participants
		ci->addChatParticipant(loggedInUser);

		INFO("DEBUG: insert");

		chats.append(ci);

		INFO("DEBUG:final countdown!");

		return QString::fromStdString(chatID);

	}


	void ChatManager::saveContactList() throw(CommunicationException, NotLoggedInException){
		sdc::SecureContainer sc;
		sdc::Security sec;

		sdc::ByteSeq c;

		// serialize
		Ice::OutputStreamPtr out = Ice::createOutputStream(ic);
		out->write(contacts);
		out->writePendingObjects();
	    out->finished(c);

	    //TODO:encryption
	    sc.data = c;

	    INFO("serialized contacts: ", string(c.begin(), c.end()));
	    INFO("raw key: ", string(privateKey.begin(), privateKey.end()));

		//sign data
		sc.signature = sec.signRSA(privateKey, sc.data);
	}

	//TOO return value
	void ChatManager::retrieveContactList() throw(CommunicationException, NotLoggedInException){

		if(!isLoggedin())
			throw(NotLoggedInException());

		try{
			sdc::SecureContainer container = session->retrieveContactList();

			//TODO: decryption
			sdc::ByteSeq data = container.data;

			if(container.signature.size() <= 0)
				throw Ice::UnmarshalOutOfBoundsException("signature empty", 0);

			////check signature
			if(!sec.verifyRSA(loggedInUser.publicKey, data, container.signature))
				throw(SignatureException());

			Ice::InputStreamPtr in = Ice::createInputStream(ic, container.data);

			in->read(contacts);
		} catch(sdc::ContactException& e){
			ERROR("ContactException!");
			throw(CommunicationException());
		} catch(sdc::SecurityException& e){
			ERROR("SecurityException!");
			throw(CommunicationException());
		} catch(Ice::UnmarshalOutOfBoundsException) {
			WARN("Seems like no contact list is stored on the server");
			//FIXME: Stub
			sdc::User a;
			sdc::User b;
			sdc::User c;
			sdc::User d;

			a.ID = "peter";
			b.ID = "jürgen";
			c.ID = "ommi";
			d.ID = "mama";

			contacts.push_back(a);
			contacts.push_back(b);
			contacts.push_back(c);
			contacts.push_back(d);
		}

	}

	/**
	 * initChat
	 *
	 * callback if another user creates new chat
	 */

	void ChatManager::initChat(const sdc::StringSeq& users, const std::string& chatID,
		const sdc::ByteSeq& key, const Ice::Current&){

		INFO("Try to initilize Chat");

		//Look for a Chatinstance. If none is found, insert a new Chatinstance into the Hashmap
		try{
			findChat(chatID);
		}catch(InvalidChatIDException& e){

			//If no Chat is found, add a new one to the chats-Hashmap
			chats.append(std::make_shared<ChatInstance>(users, chatID, key,
				boost::bind(&ChatManager::sendMessage, this, _1, _2),
				boost::bind(&ChatManager::leaveChat, this, _1)
				));
			INFO("Chat Initialized");
			return;
		}

		INFO("Chat already initialized");

	}


	/**
	 * logout
	 *
	 * logout current user
	 */

	void ChatManager::logout(void) throw(){
		
		if(session){
			try{
				session->logout();
			} catch(sdc::UserHandlingException& e){
				ERROR(e.ice_name());
			}
		}
		
	}


	/**
	 * addChatParticipant
	 *
	 * callback to add user to given chat
	 */
	void ChatManager::addChatParticipant(const sdc::User& participant,
		const std::string& chatID, const Ice::Current&){

		INFO("Try to add Chat-Participant");

		try{
			auto ci = findChat(chatID);
			ci->addChatParticipant(participant);
		}catch(InvalidChatIDException& e){
			ERROR(e.what());
		}catch(sdc::ParticipationException& e){
			ERROR(e.ice_name());
		}

		INFO("Chat-Participant added");

	}

	/**
	 * removeChatParticipant
	 *
	 * callback to remove participant from given chat
	 */

	void ChatManager::removeChatParticipant(const sdc::User& participant,
		const std::string& chatID, const Ice::Current&){

		INFO("Try to remove Chat-Participant");

		try{
			auto ci = findChat(chatID);
			ci->removeChatParticipant(participant);
		}catch(InvalidChatIDException& e){
			ERROR(e.what());
		}catch(sdc::ParticipationException& e){
			ERROR(e.ice_name());
		}

		INFO("Chat Participant removed!");
	}

	/**
	 * appendMessageToChat
	 *
	 * callback to append message to given Chat
	 */

	void ChatManager::appendMessageToChat(const sdc::ByteSeq& message, const std::string& chatID,
		const sdc::User& participant, const Ice::Current&){

		INFO("Try to append Message to Chat!");

		try{
			auto ci = findChat(chatID);
			ci->appendMessageToChat(message, participant);
		}catch(InvalidChatIDException& e){
			ERROR(e.what());
		}catch(sdc::MessageCallbackException& e){
			ERROR(e.ice_name());
		}

		INFO("Message appended to CHat!");

	}

	/**
	 * sendMessage
	 *
	 * send new message to given chat
	 */

	void ChatManager::sendMessage(const sdc::ByteSeq& message, const std::string& chatID)
		throw (CommunicationException, NotLoggedInException){

		INFO("Try to Send Message!");

		if(!isLoggedin())
			throw(NotLoggedInException());

		try{
			findChat(chatID);

			session->sendMessage(message, chatID);
		}catch(InvalidChatIDException& e){
			ERROR(e.what());
			throw(CommunicationException());
		}catch(sdc::MessageException& e){
			ERROR(e.ice_name());
			throw(CommunicationException());
		}catch(sdc::InterServerException& e){
			ERROR(e.ice_name());
			throw(CommunicationException());
		}
	}

	void ChatManager::leaveChat(const string& chatID){

		INFO("TEST LEAVE CHAT! Passiert noch nichts!" + chatID);
	}

	/*
	void ChatManager::invite(const sdc::User participant, const string& chatID, const sdc::ByteSeq& key){

		//TODO implement invite
		//TODO call session->invite();
		//TODO Add participant to user-List(in ChatInstance oder ChatManager???)
	}
	*/

	/**
	 * findChat
	 *
	 * find ChatInstance within hashmap
	 *
	 * @return ChatInstance
	 */

	std::shared_ptr<ChatInstance> ChatManager::findChat(string chatID) throw (InvalidChatIDException){
		INFO("Try to find Chat");

		for (int i = 0; i < chats.size(); i++) {
			if(chats.at(i)->id() == chatID){
				return chats.at(i);
			}
		}

		throw InvalidChatIDException();
	}

	/**
	 * getContacts
	 *
	 * @return list of all contacts
	 */

	StrListPtr ChatManager::getContacts(void){
		StrListPtr list = make_shared<QList<QString>>();

		for(size_t i; i < contacts.size(); i++){
			list->append(QString::fromStdString(contacts.at(i).ID));
		}

		return list;
	}

	/**
	 * destructor
	 *
	 * free ressources
	 */

	ChatManager::~ChatManager(){
		//TODO::saveContactList
		saveContactList();

		if(isLoggedin())
			logout();

		// if(this->ic)
		// 	ic->destroy();
	}
}
