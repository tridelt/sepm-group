#include "gtest/gtest.h"
#include "AuthenticationImpl.h"
#include "SessionImpl.h"
#include "DBPool.h"
#include "gmock/gmock.h"
#include "Logging.h"
#include "IceMocks.h"
#include <boost/optional.hpp>

using namespace soci;
using ::testing::_;
using ::testing::Return;
using ::testing::AtLeast;

class SessionTest : public ::testing::Test {
 protected:
  virtual void SetUp() {
    // important - in-memory db is much faster
    pool = DBPool::TestPool();
    // make sure tests are quiet
    logger.clearSinks();
    soci::session sql(pool->getPool());
    sql << "DROP TABLE IF EXISTS users;";
    password = "secret";
    u.ID = "hello";
    auth = new AuthenticationImpl(&server_mock, pool);
    session = new SessionImpl(u, pool);
  }

  virtual void TearDown() {
    delete pool;
    delete auth;
    delete session;
  }

  DBPool *pool;
  Ice::Current curr;
  Ice::Identity id;
  string password;
  sdc::User u;
  AuthenticationImpl *auth;
  SessionImpl *session;
  IceServerMock server_mock;  // used by auth to expose the SessionI
};

TEST_F(SessionTest, ThrowAfterLogout) {
  session->logout(curr);

  auto container = sdc::SecureContainer {
    sdc::ByteSeq(), sdc::ByteSeq()
  };

  ASSERT_THROW(session->retrieveUser("someone", curr), sdc::SessionException);
  ASSERT_THROW(session->initChat(curr), sdc::SessionException);
  ASSERT_THROW(session->leaveChat("some", curr), sdc::SessionException);
  ASSERT_THROW(session->invite(u, "some", sdc::ByteSeq(), curr), sdc::SessionException);
  ASSERT_THROW(session->sendMessage(sdc::ByteSeq(), "some", curr), sdc::SessionException);
  ASSERT_THROW(session->deleteUser(u, curr), sdc::SessionException);
  ASSERT_THROW(session->saveLog("some", 4, container, curr), sdc::SessionException);
  ASSERT_THROW(session->retrieveLoglist(curr), sdc::SessionException);
  ASSERT_THROW(session->retrieveLog("some", 4, curr), sdc::SessionException);
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

  session->saveLog("chan", 10, log_entry, curr);

  ASSERT_EQ(log_entry, session->retrieveLog("chan", 10, curr));
}


TEST_F(SessionTest, CanRetrieveLoglist) {
  string a = "my data";
  string b = "my signature";
  auto log_entry = sdc::SecureContainer {
    sdc::ByteSeq(a.begin(), a.end()),
    sdc::ByteSeq(b.begin(), b.end())
  };

  sdc::ChatlogEntry entry { "chan", 10 };

  session->saveLog("chan", 10, log_entry, curr);

  auto loglist = session->retrieveLoglist(curr);

  ASSERT_EQ(1, loglist.size());
  ASSERT_EQ(entry, loglist[0]);
}
