#include "gtest/gtest.h"
#include "AuthenticationImpl.h"
#include "SessionImpl.h"
#include "DBPool.h"
#include "gmock/gmock.h"
#include "Logging.h"
#include "IceMocks.h"
#include <boost/optional.hpp>
#include "config.h"

using namespace soci;
using ::testing::_;
using ::testing::Return;
using ::testing::AtLeast;

class SessionTest : public ::testing::Test {
 protected:
  virtual void SetUp() {
    // important - in-memory db is much faster
    pool.reset(DBPool::TestPool());
    // make sure tests are quiet
    logger.clearSinks();
    soci::session sql(pool->getPool());
    sql << "DROP TABLE IF EXISTS users;";
    password = "secret";
    u.ID = "hello@" + Config::hostname();
    cmgr.reset(new ChatManager());
    smgr.reset(new SessionManager());
    server_mock = new ::testing::NiceMock<IceServerMock>(pool, cmgr, smgr);
    auth.reset(new AuthenticationImpl(server_mock));
    CallbackMock callback_mock;
    auto callback_fake = shared_ptr<ChatClientCallbackInd>(new CallbackFake(&callback_mock));
    session.reset(new SessionImpl(u, server_mock, callback_fake));
  }

  virtual void TearDown() {
    delete server_mock;
    pool.reset();
    auth.reset();
    session.reset();
    cmgr.reset();
    smgr.reset();
  }

  Ice::Current curr;
  Ice::Identity id;
  string password;
  sdc::User u;
  shared_ptr<AuthenticationImpl> auth;
  shared_ptr<DBPool> pool;
  shared_ptr<ChatManager> cmgr;
  shared_ptr<SessionManager> smgr;
  shared_ptr<SessionImpl> session;
  ::testing::NiceMock<IceServerMock> *server_mock;  // used by auth to expose the SessionI
};

TEST_F(SessionTest, ThrowAfterLogout) {
  session->logout(curr);

  auto container = sdc::SecureContainer {
    sdc::ByteSeq(), sdc::ByteSeq()
  };

  ASSERT_THROW(session->retrieveUser("someone@" + Config::hostname(), curr), sdc::SessionException);
  ASSERT_THROW(session->initChat(curr), sdc::SessionException);
  ASSERT_THROW(session->leaveChat("some@" + Config::hostname(), curr), sdc::SessionException);
  ASSERT_THROW(session->invite(u, "some@" + Config::hostname(), sdc::ByteSeq(), curr), sdc::SessionException);
  ASSERT_THROW(session->sendMessage(sdc::ByteSeq(), "some@" + Config::hostname(), curr), sdc::SessionException);
  ASSERT_THROW(session->deleteUser(u, curr), sdc::SessionException);
  ASSERT_THROW(session->saveLog("some@" + Config::hostname(), 4, container, curr), sdc::SessionException);
  ASSERT_THROW(session->retrieveLoglist(curr), sdc::SessionException);
  ASSERT_THROW(session->retrieveLog("some@" + Config::hostname(), 4, curr), sdc::SessionException);
  ASSERT_THROW(session->saveContactList(container, curr), sdc::SessionException);
  ASSERT_THROW(session->retrieveContactList(curr), sdc::SessionException);
}


TEST_F(SessionTest, CanSaveContactList) {
  string a = "my data";
  string b = "my signature";
  auto contacts = sdc::SecureContainer {
    sdc::ByteSeq(a.begin(), a.end()),
    sdc::ByteSeq(b.begin(), b.end())
  };

  session->saveContactList(contacts, curr);

  ASSERT_EQ(contacts, session->retrieveContactList(curr));
}

TEST_F(SessionTest, CanSaveLog) {
  string a = "my data";
  string b = "my signature";
  auto log_entry = sdc::SecureContainer {
    sdc::ByteSeq(a.begin(), a.end()),
    sdc::ByteSeq(b.begin(), b.end())
  };

  session->saveLog("chan@" + Config::hostname(), 10, log_entry, curr);

  ASSERT_EQ(log_entry, session->retrieveLog("chan@" + Config::hostname(), 10, curr));
}


TEST_F(SessionTest, CanRetrieveLoglist) {
  string a = "my data";
  string b = "my signature";
  auto log_entry = sdc::SecureContainer {
    sdc::ByteSeq(a.begin(), a.end()),
    sdc::ByteSeq(b.begin(), b.end())
  };

  sdc::ChatlogEntry entry { "chan@" + Config::hostname(), 10 };

  session->saveLog("chan@" + Config::hostname(), 10, log_entry, curr);

  auto loglist = session->retrieveLoglist(curr);

  ASSERT_EQ(1, loglist.size());
  ASSERT_EQ(entry, loglist[0]);
}

TEST_F(SessionTest, CanDeleteOwnAccount) {
  auth->registerUser(u, password, curr);
  session->deleteUser(u, curr);

  Ice::ObjectPrx magicProxy;

  CallbackMock callback_mock;
  auto callback_fake = shared_ptr<ChatClientCallbackInd>(new CallbackFake(&callback_mock));

  // login() should test the validity of the connection before allowing a login
  // first it has to create a proxy for the connection
  ON_CALL(*server_mock, callbackForID(_, _)).WillByDefault(Return(callback_fake));
  EXPECT_CALL(*server_mock, callbackForID(_, _)).Times(0);
  // then call echo on the client callback
  EXPECT_CALL(callback_mock, echo(_)).Times(0);
  // neither should be called since the account was deleted

  // therefore, it also shouldn't call exposeObject to expose the SessionI
  ON_CALL(*server_mock, exposeObject(_, _)).WillByDefault(Return(magicProxy));
  EXPECT_CALL(*server_mock, exposeObject(_, _)).Times(0);

  // shouldn't be able to login anymore - account was just deleted
  ASSERT_THROW(auth->login(u, password, id, curr), sdc::AuthenticationException);


  auto container = sdc::SecureContainer {
    sdc::ByteSeq(), sdc::ByteSeq()
  };

  // also shouldn't be able to do anything else
  ASSERT_THROW(session->retrieveUser("someone@" + Config::hostname(), curr), sdc::SessionException);
  ASSERT_THROW(session->initChat(curr), sdc::SessionException);
  ASSERT_THROW(session->leaveChat("some@" + Config::hostname(), curr), sdc::SessionException);
  ASSERT_THROW(session->invite(u, "some@" + Config::hostname(), sdc::ByteSeq(), curr), sdc::SessionException);
  ASSERT_THROW(session->sendMessage(sdc::ByteSeq(), "some@" + Config::hostname(), curr), sdc::SessionException);
  ASSERT_THROW(session->deleteUser(u, curr), sdc::SessionException);
  ASSERT_THROW(session->saveLog("some@" + Config::hostname(), 4, container, curr), sdc::SessionException);
  ASSERT_THROW(session->retrieveLoglist(curr), sdc::SessionException);
  ASSERT_THROW(session->retrieveLog("some@" + Config::hostname(), 4, curr), sdc::SessionException);
  ASSERT_THROW(session->saveContactList(container, curr), sdc::SessionException);
  ASSERT_THROW(session->retrieveContactList(curr), sdc::SessionException);
}

TEST_F(SessionTest, CantDeleteOtherAccount) {
  sdc::User u2;
  u2.ID = "world@" + Config::hostname();

  auth->registerUser(u, password, curr);
  auth->registerUser(u2, password, curr);

  ASSERT_THROW(session->deleteUser(u2, curr), sdc::UserHandlingException);
}

TEST_F(SessionTest, CanRetrieveUser) {
  auth->registerUser(u, password, curr);

  ASSERT_EQ(u, session->retrieveUser(u.ID, curr));
}

TEST_F(SessionTest, CantLeaveChatUserIsNotIn) {
  ASSERT_THROW(session->leaveChat("chan@" + Config::hostname(), curr), sdc::SessionException);
}

TEST_F(SessionTest, ChatNamesShouldBeUnique) {
  ASSERT_NE(session->initChat(curr), session->initChat(curr));
}


TEST_F(SessionTest, CanOnlySendToExistingChannels) {
  ASSERT_THROW(session->sendMessage(sdc::ByteSeq(), "someChan", curr), sdc::MessageException);
}
