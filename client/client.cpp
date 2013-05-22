
//std
#include <iostream>

//boost
#include <boost/program_options.hpp>
#include <boost/bind.hpp>
#include <boost/filesystem.hpp>

//Qt
#include <QDir>

//helper
#include "sdcHelper.h"
#include "Security.h"

//homebrew
#include "ChatManager.h"
#include "ExitHandler.h"
#include "Logging.h"
#include "ui_ChatMainWindow.h"
#include "ui_ChatWelcomeWindow.h"

namespace po = boost::program_options;
using namespace std;
using namespace cm;

void shutdown(){
  QApplication::quit();
}

void addContact(QPushButton button){

  ui->HorizontalLayout->addWidget(button);
  button->show();
}

int main(int argc, char** argv) {
 QApplication app(argc, argv);

  sdc::Security sec;

  //open main window
  QMainWindow *mw = new QMainWindow;
  Ui_ChatMainWindow ui;

  ui.setupUi(mw);




  mw->show();

  //start Communication Manager
  //TODO: Dialog to set host etc if its not already set.
  QString host("sepm.furidamu.org");
  QString user("kaesekrainer@sepm.furidamu.org");
  QString pw("123");
  int port = 1337;
  QString cert(QDir::homePath() + "/.config/sdc/ca.crt");
  QString publicKey_path(QDir::homePath() + "/.config/sdc/public_sepm.furidamu.org.pem");
  QString privateKey_path(QDir::homePath() + "/.config/sdc/private_sepm.furidamu.org.pem");

  //create testuser
  sdc::User u;
  u.ID = user.toStdString();
  sdc::ByteSeq privateKey;
  


  //check keys
  if (!boost::filesystem::exists(publicKey_path.toStdString())){
    sec.genRSAKey(u.publicKey, privateKey);

    //store keys
    sec.savePrivKey(privateKey, privateKey_path);
    sec.savePubKey(u.publicKey, publicKey_path);

  } else{
    u.publicKey = sec.readPubKey(publicKey_path);
  }

  ChatManager* cm;
  cm = new ChatManager(host.toStdString(), port, cert.toStdString());

  //test connectivity
  if(cm->isOnline()){
    INFO("Connection established");
  }else{
    ERROR("Could not connect!");
    shutdown();
  }

  //register
  try{
    cm->registerUser(u, pw);
  } catch(AlreadyRegisteredException& e){
    INFO("already registered");
  }


  //login
  try{
    cm->login(u, pw);
  } catch(CommunicationException& e){
    INFO("could not login!");
  }

  //init new chat
  //try{
    std::string chatid = cm->initChat().toStdString();
  //} catch(){

  //}

  std::string msg = "hi, du";

  sdc::ByteSeq data(msg.begin(), msg.end());

  cm->sendMessage(data ,chatid);

  //catch contact list




  //open login/logout dialog if no active user set
  if(true){
    INFO("look, user is active.");

    //TODO: open active conversations
    //TODO: load contacts
  }else{
    //show welcome dialog
    //QDialog *wel = new QDialog;
    //Ui_ChatWelcomeWindow cw_ui;
    //cw_ui.setupUi(wel);
    //wel->show();

    //set user in comunicationmanager
    //entweder register und login oder nur login aufrufen

    //TODO: register
    //cm->registerUser("christ", "123", "/home/chris/.config/sdc/public_chris@hotz@sepm.furidamu.org.pem");s
  }

  /*
   *
   * login
   *
   */

   //add user to props
   //cm->addUser(u);

   //cm->registerUser(user, pw);

   //cm->login(u, pw);

   //cm->initChat();

   try{
    cm->logout();
   }catch(NotLoggedInException& e){
    ERROR("hey, you are not logged in! ");
   }

   

    ExitHandler::i()->setHandler([](int) {
      // called when SIGINT (eg by Ctrl+C) is received
      // do cleanup

      // bad - cout not guaranteed to work, since not reentrant
      // this is just to show the handler is working
      INFO(" Got signal .. terminating");
      shutdown();
      
    });


  return app.exec();
}


