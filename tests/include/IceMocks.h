
#include "IceServerI.h"
#include "gmock/gmock.h"

class IceServerMock : public IceServerI {
public:
  MOCK_METHOD2(exposeObject, Ice::ObjectPrx (const Ice::ObjectPtr &o,
                                const string &id));
};

class IceConnectionMock : public Ice::Connection {
public:
  MOCK_CONST_METHOD1(createProxy, Ice::ObjectPrx (const Ice::Identity &id));
  MOCK_METHOD1(close, void (bool));
  MOCK_METHOD1(setAdapter, void (const Ice::ObjectAdapterPtr&));
  MOCK_CONST_METHOD0(getAdapter, Ice::ObjectAdapterPtr ());
  MOCK_METHOD0(flushBatchRequests, void ());
  MOCK_CONST_METHOD0(getEndpoint, Ice::EndpointPtr ());
  MOCK_METHOD0(begin_flushBatchRequests, Ice::AsyncResultPtr ());
  MOCK_METHOD2(begin_flushBatchRequests, Ice::AsyncResultPtr (const Ice::CallbackPtr&, const Ice::LocalObjectPtr&));
  MOCK_METHOD2(begin_flushBatchRequests, Ice::AsyncResultPtr (const Ice::Callback_Connection_flushBatchRequestsPtr&, const Ice::LocalObjectPtr&));
  MOCK_METHOD1(end_flushBatchRequests, void (const Ice::AsyncResultPtr &));
  MOCK_CONST_METHOD0(type, string ());
  MOCK_CONST_METHOD0(timeout, Ice::Int ());
  MOCK_CONST_METHOD0(toString, string ());
  MOCK_CONST_METHOD0(getInfo, Ice::ConnectionInfoPtr ());
};


class CallbackMock : public sdc::ChatClientCallbackIPrx {
  string echo(string s) { return s; }
};
