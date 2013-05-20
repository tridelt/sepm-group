
/**
 * servernotrespinding on timeout or login failure
 */

#include "ChatManager.h"

//boost
#include <boost/filesystem.hpp>

//ice
#include <IceUtil/IceUtil.h>

//helper
 #include "Security.h"

//homebrew
#include "Logging.h"

namespace cm{

	ChatManager::ChatManager(std::string host, int port, std::string cert_path) 
		throw (ServerUnavailableException, FileNotFoundException):
		session(NULL),keysize(512),loggedInUser(NULL){
		INFO("try to establish connection");

		this->host = host;
		this->port = port;
		this->cert_path = cert_path;

		props = Ice::createProperties();

		//try to find file
		if (!boost::filesystem::exists(cert_path)){
	    	ERROR("cert not found! " + cert_path);

	    	throw(FileNotFoundException());
		}

		///init ice

		try {
			//TODO props only local?
			INFO("try");
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
		  	//FIXME:exctract message from e
			ERROR(e);

			throw(ServerUnavailableException());
		  }

		/// try to create Proxy
	}

	/**
	 * registerUser
	 *
	 * register new chat user
	 */

	void ChatManager::registerUser(sdc::User user, QString pwd) throw (AlreadyRegisteredException, CommunicationException){
		try{
			sdc::AuthenticationIPrx auth = sdc::AuthenticationIPrx::checkedCast(base);

			//TODO: test connectivity and maybe throw new CommunicationException

			auth->registerUser(user, pwd.toStdString());
		} catch(const sdc::AuthenticationException& e){
			//TODO: log entry
			//TODO: Errorhandling
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
		if(!session)
			return false;
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
		} catch(const sdc::AuthenticationException& e){
			//FIXME: exctract error message
			ERROR(e);

		}

		loggedInUser = &user;
	}

	/** 
	 * initChat
	 *
	 * create new Chat
	 *
	 * @return chatID
	 */

	QString ChatManager::initChat() throw (CommunicationException, NotLoggedInException, InvalidChatIDException){
		std::string chatID;

		//test if currently logged in
		if(!isLoggedin())
			throw(NotLoggedInException());

		//create new Chat
		try{
			chatID = session->initChat();
		} catch(sdc::SessionException& e){
			//TODO Exception handling
		}

		//id validation
		if(chatID == "")
			throw(InvalidChatIDException());		

		//Security helper
		sdc::Security *sec = new sdc::Security();

		//Chat participants
		sdc::StringSeq users;

		//generate session-key
		sdc::ByteSeq key = sec->genAESKey(keysize);

		ChatInstance *ci = new ChatInstance(users, chatID, key);

		//add logged in User to the Chat-Participants
		ci->addChatParticipant(*loggedInUser);

		chats->insert(QString::fromStdString(chatID), ci);

		return QString::fromStdString(chatID);
	}

	/**
	 * initChat
	 *
	 * callback if another user creates new chat
	 */

	void ChatManager::initChat(const sdc::StringSeq& users, const std::string& chatID, const sdc::ByteSeq& key, const Ice::Current&){

		//Look for a Chatinstance. If none is found, insert a new Chatinstance into the Hashmap
		try{
			findChat(chatID);
		}catch(InvalidChatIDException& e){
			chats->insert(QString::fromStdString(chatID), new ChatInstance(users, chatID, key));
			return;
		}

		INFO("Chat already initialized");

	}


	/**
	 * logout
	 *
	 * logout current user
	 */

	void ChatManager::logout(void) throw (CommunicationException, NotLoggedInException){
		//test if currently logged in
		if(!isLoggedin())
			throw(NotLoggedInException());

		try{
			session->logout();
		} catch(sdc::UserHandlingException& e){
			//FIXME: extract error message
			ERROR(e);
		}

		//TODO loggedInUser, den man beim login gesetzt hat, wieder auf NULL setzen
	}

	
	/**
	 * addChatParticipant
	 *
	 * callback to add user to given chat
	 */
	void ChatManager::addChatParticipant(const sdc::User& participant, const std::string& chatID, const Ice::Current&){

		ChatInstance *ci;

		try{
			ci = findChat(chatID);
		}catch(InvalidChatIDException& e){
			ERROR(e.what());
		}		
		
		ci->addChatParticipant(participant);
	}

	/**
	 * removeChatParticipant
	 *
	 * callback to remove participant from given chat
	 */

	void ChatManager::removeChatParticipant(const sdc::User& participant, const std::string& chatID, const Ice::Current&){

		ChatInstance *ci;

		try{
			ci = findChat(chatID);
		}catch(InvalidChatIDException& e){
			ERROR(e.what());
		}
		
		ci->removeChatParticipant(participant);
	}

	/**
	 * appendMessageToChat
	 *
	 * callback to append message to given Chat
	 */

	void ChatManager::appendMessageToChat(const sdc::ByteSeq& message, const std::string& chatID, const sdc::User& participant, const Ice::Current&){
		
		ChatInstance *ci;

		try{
			ci = findChat(chatID);
		}catch(InvalidChatIDException& e){
			ERROR(e.what());
		}

		ci->appendMessageToChat(message, participant);

	}

	/**
	 * sendMessage
	 *
	 * send new message to given chat
	 */

	void ChatManager::sendMessage(const sdc::ByteSeq& message, const std::string& chatID) throw (CommunicationException, NotLoggedInException){

		if(!isLoggedin())
			throw(new NotLoggedInException());

		try{
			findChat(chatID);
		}catch(InvalidChatIDException& e){
			ERROR(e.what());
		}

		session->sendMessage(message, chatID);

	}
	
	/**
	 * findChat
	 *
	 * find ChatInstance within hashmap
	 *
	 * @return ChatInstance
	 */

	ChatInstance* ChatManager::findChat(string chatID) throw (InvalidChatIDException){

		QMultiHash<QString, ChatInstance*>::iterator i = chats->find(QString::fromStdString(chatID));
		
		ChatInstance *ci;

		//find returns an iterator. Therefor the while-loop.
		//Break, if the first occurence of the desired chat is found		
		while(i != chats->end() && i.key() == QString::fromStdString(chatID)) {
		     ci = i.value();
		     break;
		}

		//If no chat is found
		//TODO checken ,obs so passt
		if(ci == NULL){
			throw(new InvalidChatIDException);
		}

		return ci;
	}

	/**
	 * destructor
	 *
	 * free ressources
	 */

	ChatManager::~ChatManager(){

		//TODO: close connection

		//TODO: free member attributes

		//TODO: free ice
		if(this->ic)
			ic->destroy();
	}
}