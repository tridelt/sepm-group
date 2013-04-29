#include "SecureDistributedChat.h"
#include <set>
#include <string>
#include <boost/thread.hpp>

using namespace std;

class ChatDB {
public:
  static ChatDB* i();
  set<string> usersForChat(const string &chat);
  sdc::InterServerI* serverForUser(const string &user);
  sdc::User userForString(const string &user);
  string decryptMsgForChat(const string &chat, const sdc::ByteSeq &msg);

  void setKeyForChat(const string &chat, const sdc::ByteSeq &key);
  void addUserToChat(const string &chat, const string &user);
  void removeUserFromChat(const string &chat, const string &user);

  sdc::ByteSeq publicKey;
  sdc::ByteSeq privateKey;
private:
  ChatDB();
  static ChatDB* instance;
  static boost::mutex mutex;

  map<string, set<string>> chatUsers; /** Map[Chat, List[User]] */
  map<string, sdc::InterServerI*> userServers; /** Map[User, Server] */
  map<string, sdc::User> users; /** Map[UserStr, sdc::User] */
  map<string, sdc::ByteSeq> chatKeys; /** Map[Chat, AES-Key] */
};
