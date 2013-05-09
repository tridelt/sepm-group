#ifndef ICE_MOCKS_H
#define ICE_MOCKS_H

#include "IceServerI.h"
#include "gmock/gmock.h"

using ::testing::Invoke;
using ::testing::_;

class IceServerMock : public IceServerI {
public:
  MOCK_METHOD2(exposeObject, Ice::ObjectPrx (const Ice::ObjectPtr &o,
                                const string &id));

  MOCK_METHOD2(callbackForID, shared_ptr<ChatClientCallbackInd> (
    const Ice::Identity &callbackID, const Ice::ConnectionPtr &con));
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
  CallbackFake(const CallbackFake* o) : _mock(o->_mock) {}
private:
  CallbackMock* _mock;
};

#endif
