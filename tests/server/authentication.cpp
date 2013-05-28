#include "gtest/gtest.h"
#include "AuthenticationImpl.h"
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

class AuthenticationTest : public ::testing::Test {
 protected:
  virtual void SetUp() {
    // important - in-memory db is much faster
    pool.reset(DBPool::TestPool());
    // make sure tests are quiet
    logger.clearSinks();
    session sql(pool->getPool());
    sql << "DROP TABLE IF EXISTS users;";
    password = "secret";
    u.ID = "hello@" + Config::hostname();
    cmgr.reset(new ChatManager());
    smgr.reset(new SessionManager());
    ismgr.reset(new InterServerManager("ca.crt"));
    server_mock = new ::testing::NiceMock<IceServerMock>(pool, cmgr, smgr, ismgr);
    auth.reset(new AuthenticationImpl(server_mock));
  }

  virtual void TearDown() {
    delete server_mock;
    pool.reset();
    auth.reset();
    cmgr.reset();
    smgr.reset();
    ismgr.reset();
  }

  Ice::Current curr;
  Ice::Identity id;
  string password;
  sdc::User u;
  shared_ptr<AuthenticationImpl> auth;
  shared_ptr<DBPool> pool;
  shared_ptr<ChatManager> cmgr;
  shared_ptr<SessionManager> smgr;
  shared_ptr<InterServerManager> ismgr;
  ::testing::NiceMock<IceServerMock> *server_mock;  // used by auth to expose the SessionI
};

TEST_F(AuthenticationTest, CanEcho) {
  ASSERT_EQ("test_str", auth->echo("test_str", curr));
}


TEST_F(AuthenticationTest, CanRegisterAndLogin) {
  Ice::ObjectPrx magicProxy;

  auth->registerUser(u, password, curr);

  CallbackMock callback_mock;
  auto callback_fake = shared_ptr<ChatClientCallbackInd>(new CallbackFake(&callback_mock));

  // login() should test the validity of the connection before allowing a login
  // first it has to create a proxy for the connection
  EXPECT_CALL(*server_mock, callbackForID(_, _)).WillOnce(Return(callback_fake));
  // then call echo on the client callback
  EXPECT_CALL(callback_mock, echo(_)).Times(AtLeast(1));

  // login has to call exposeObject to expose the SessionI
  EXPECT_CALL(*server_mock, exposeObject(_, _)).WillOnce(Return(magicProxy));

  // check that the magicProxy specified above is returned here
  auth->login(u, password, id, curr);
}

TEST_F(AuthenticationTest, CantRegisterTwice) {
  auth->registerUser(u, "secret", curr);
  ASSERT_THROW(auth->registerUser(u, password, curr), sdc::AuthenticationException);
}

TEST_F(AuthenticationTest, AvoidCleartextPasswords) {
  auth->registerUser(u, password, curr);

  session sql(pool->getPool());

  boost::optional<string> id;
  sql << "SELECT id FROM users WHERE pw = :pw",
    into(id), use(password);

  ASSERT_FALSE(id.is_initialized());
}

TEST_F(AuthenticationTest, NoLoginWithoutRegister) {
  ASSERT_THROW(auth->login(u, password, id, curr), sdc::AuthenticationException);
}

TEST_F(AuthenticationTest, NoLoginWithBadPassword) {
  auth->registerUser(u, password, curr);
  ASSERT_THROW(auth->login(u, "", id, curr), sdc::AuthenticationException);
  ASSERT_THROW(auth->login(u, "password", id, curr), sdc::AuthenticationException);
}
