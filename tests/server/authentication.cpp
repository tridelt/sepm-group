#include "gtest/gtest.h"
#include "AuthenticationImpl.h"
#include "DBPool.h"
#include "gmock/gmock.h"
#include "Logging.h"
#include "IceMocks.h"

Ice::ObjectPrx magicProxy;
using namespace soci;
using ::testing::_;
using ::testing::Return;


class AuthenticationTest : public ::testing::Test {
 protected:
  virtual void SetUp() {
    pool = new DBPool("test_db");
    // make sure tests are quiet
    logger.clearSinks();
    session sql(pool->getPool());
    sql << "DROP TABLE IF EXISTS users;";
    password = "secret";
    u.ID = "hello";
    auth = new AuthenticationImpl(&server_mock, pool);
  }

  virtual void TearDown() {
    delete pool;
    delete auth;
  }

  DBPool *pool;
  Ice::Current curr;
  Ice::Identity id;
  string password;
  sdc::User u;
  AuthenticationImpl *auth;
  IceServerMock server_mock;  // used by auth to expose the SessionI
};

TEST_F(AuthenticationTest, CanEcho) {
  ASSERT_EQ("test_str", auth->echo("test_str", curr));
}


TEST_F(AuthenticationTest, CanRegisterAndLogin) {
  auth->registerUser(u, password, curr);

  IceConnectionMock connection_mock;
  CallbackMock callback_mock;

  // login() should test the validity of the connection before allowing a login
  EXPECT_CALL(connection_mock, createProxy(id)).WillOnce(Return(callback_mock));

  // login has to call exposeObject to expose the SessionI
  EXPECT_CALL(server_mock, exposeObject(_, _)).WillOnce(Return(magicProxy));

  // check that the magicProxy specified above is returned here
  ASSERT_EQ(magicProxy, auth->login(u, password, id, curr));
}

TEST_F(AuthenticationTest, CantRegisterTwice) {
  auth->registerUser(u, "secret", curr);
  ASSERT_THROW(auth->registerUser(u, password, curr), sdc::AuthenticationException);
}
