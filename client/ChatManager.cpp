
/**
 * servernotrespinding on timeout or login failure
 */

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
		session(NULL) {
		INFO("try to establish connection");

		this->host = host;
		this->port = port;
		this->cert_path = cert_path;

		props = Ice::createProperties();

		//try to find file
		if (!boost::filesystem::exists(cert_path)){
	    	ERROR("cert not found! " + cert_path);

	    	throw(new FileNotFoundException());
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

			throw(new ServerUnavailableException());
		  } catch (const Ice::PluginInitializationException& e) {
			ERROR("failed to initialize SSL plugin - are you using the correct certificate?");

			//FIXME: find more suitable exception
			throw(new ServerUnavailableException());
		  } catch (const Ice::EndpointParseException& e) {
			ERROR("Failed to create endpoint, check server and port: " 
				+ host + "/" 
				+ boost::lexical_cast<string>(port));

			throw(new ServerUnavailableException());
		  } catch (const Ice::DNSException& e) {
			ERROR("error resolving hostname");

			throw(new ServerUnavailableException());
		  }catch (const Ice::Exception& e) {
		  	//FIXME:exctract message from e
			ERROR(e);

			throw(new ServerUnavailableException());
		  }

		/// try to create Proxy
	}

	void ChatManager::registerUser(sdc::User user, QString pwd) throw (AlreadyRegisteredException, CommunicationException){
		try{
			sdc::AuthenticationIPrx auth = sdc::AuthenticationIPrx::checkedCast(base);

			//TODO: test connectivity and maybe throw new CommunicationException

			auth->registerUser(user, pwd.toStdString());
		} catch(const sdc::AuthenticationException& e){
			//TODO: log entry
			//TODO: Errorhandling
			throw(new AlreadyRegisteredException());
		}
	}

	/**
	 * 
	 *
	 * @return true if connected
	 */
	bool ChatManager::isOnline(void){
		try{
			base->ice_ping();
		} catch (const Ice::Exception& e) {
			return false;
		}

		return true;
	}

	bool ChatManager::isLoggedin(void){
		if(!session)
			return false;
		return true;
	}




	void ChatManager::login(sdc::User user, std::string pwd) throw (CommunicationException){
		try{
			sdc::AuthenticationIPrx auth = sdc::AuthenticationIPrx::checkedCast(base);

			Ice::Identity ident;
			Ice::ObjectAdapterPtr adapter = ic->createObjectAdapter("");
			ident.name = IceUtil::generateUUID();
			ident.category = "";

			//Callback
			//auto cb = new ChatClientCallbackImpl;
			adapter->add((sdc::ChatClientCallbackI*)this, ident);
			adapter->activate();
			base->ice_getConnection()->setAdapter(adapter);

			session = auth->login(user, pwd, ident);
		} catch(const sdc::AuthenticationException& e){
			//FIXME: exctract error message
			ERROR(e);

		}
	}


	QString ChatManager::initChat() throw (CommunicationException, NotLoggedInException, InvalidChatIDException){
		std::string chatID;

		//test if currently logged in
		if(!isLoggedin())
			throw(new NotLoggedInException());

		//create new Chat
		try{
			chatID = session->initChat();
		} catch(sdc::SessionException& e){
			//TODO: exceptionhandling
		}


		//id validation
		if(chatID == "")
			throw(new InvalidChatIDException());

		return QString::fromStdString(chatID);
	}


	void ChatManager::logout(void) throw (CommunicationException, NotLoggedInException){
		//test if currently logged in
		if(!isLoggedin())
			throw(new NotLoggedInException());

		try{
			session->logout();
		} catch(sdc::UserHandlingException& e){
			//FIXME: extract error message
			ERROR(e);
		}
	}

	void ChatManager::sendMessage(const sdc::ByteSeq& msg, const string& chatID) throw (CommunicationException, NotLoggedInException, InvalidChatIDException){
		//test if currently logged in
		if(!isLoggedin())
			throw(new NotLoggedInException());

		//TODO: Error handly. check if logged in.
		session->sendMessage(msg, chatID);
	}

	ChatManager::~ChatManager(){

		//TODO: close connection

		//TODO: free member attributes

		//TODO: free ice
		if(this->ic)
			ic->destroy();
	}
}