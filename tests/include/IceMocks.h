#ifndef ICE_MOCKS_H
#define ICE_MOCKS_H

#include "IceServerI.h"
#include "gmock/gmock.h"

using ::testing::Invoke;
using ::testing::_;

class IceServerMock : public IceServerI {
public:
  IceServerMock(shared_ptr<DBPool> pool, shared_ptr<ChatManager> cmgr, shared_ptr<SessionManager> smgr)
    : db_pool(pool), chat_mgr(cmgr), session_mgr(smgr) {
    //TODO: may have to return false for InterServer tests
    ON_CALL(*this, isLocal(_)).WillByDefault(testing::Return(true));
    ON_CALL(*this, getDBPool()).WillByDefault(testing::Return(db_pool));
    ON_CALL(*this, getChats()).WillByDefault(testing::Return(chat_mgr));
    ON_CALL(*this, getSessions()).WillByDefault(testing::Return(session_mgr));
  }

  MOCK_METHOD2(exposeObject, Ice::ObjectPrx (const Ice::ObjectPtr &o,
                                const string &id));

  MOCK_METHOD2(callbackForID, shared_ptr<ChatClientCallbackInd> (
    const Ice::Identity &callbackID, const Ice::ConnectionPtr &con));

  MOCK_METHOD1(isLocal, bool(const string&));

  MOCK_METHOD0(getDBPool, shared_ptr<DBPool>());
  MOCK_METHOD0(getChats, shared_ptr<ChatManager>());
  MOCK_METHOD0(getSessions, shared_ptr<SessionManager>());

  shared_ptr<DBPool> db_pool;
  shared_ptr<ChatManager> chat_mgr;
  shared_ptr<SessionManager> session_mgr;
};


class CallbackMock {
public:
  MOCK_METHOD1(echo, string (const string&));
  MOCK_METHOD3(initChat, void (const sdc::StringSeq&, const string&, const sdc::ByteSeq&));
  MOCK_METHOD2(addChatParticipant, void (const sdc::User&, const string&));
  MOCK_METHOD2(removeChatParticipant, void (const sdc::User&, const string&));
  MOCK_METHOD3(appendMessageToChat, void (const sdc::ByteSeq&, const string&, const sdc::User&));
};

class CallbackFake : public ChatClientCallbackInd {
public:
  string echo(const string &s) {
    _mock->echo(s);
    return s;
  }
  void initChat(const sdc::StringSeq &, const string &, const sdc::ByteSeq &) {}
  void addChatParticipant(const sdc::User &, const string &) {}
  void removeChatParticipant(const sdc::User &, const string &) {}
  void appendMessageToChat(const sdc::ByteSeq &, const string &, const sdc::User &) {}

  CallbackFake(CallbackMock* m) : _mock(m) {}
private:
  CallbackMock* _mock;
};

#endif
