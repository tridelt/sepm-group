class ChatDB {
public:
  vector<string> usersForChat(string chat);
  sdc::InterServerI serverForUser(string user);
};
