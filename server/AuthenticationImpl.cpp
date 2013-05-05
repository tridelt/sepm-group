#include "AuthenticationImpl.h"
#include <iostream>
#include "DBPool.h"
#include "sdcHelper.h"
#include <boost/optional.hpp>
#include "SessionImpl.h"

using namespace std;
using namespace soci;

void AuthenticationImpl::registerUser(const sdc::User &u, const string &pw,
  const Ice::Current&) {
  if(!sdc::sdcHelper::isValidID(u.ID))
    throw sdc::AuthenticationException("invalid ID");

  session sql(DBPool::i()->getPool());

  sql << "CREATE TABLE IF NOT EXISTS users(id text, pw text, pubkey text)";

  auto name = sdc::sdcHelper::getNameFromID(u.ID);
  boost::optional<string> id;
  sql << "SELECT id FROM users WHERE id = :id",
    into(id), use(name);

  if(id.is_initialized())
    throw sdc::AuthenticationException("ID already exists");

  // TODO: properly hash the password (bcrypt)
  string pubkey(u.publicKey.begin(), u.publicKey.end());
  sql << "INSERT INTO users(id, pw, pubkey) VALUES (:id, :pw, :pubkey)",
    use(name), use(pw), use(pubkey);

  // don't log passwords!
  cout << "Registered " << u.ID << endl;
}

sdc::SessionIPrx AuthenticationImpl::login(const sdc::User &u, const string &pw,
    const Ice::Identity&, const Ice::Current&) {
  if(!sdc::sdcHelper::isValidID(u.ID))
    throw sdc::AuthenticationException("invalid ID");

  session sql(DBPool::i()->getPool());

  sql << "CREATE TABLE IF NOT EXISTS users(id text, pw text, pubkey text)";

  auto name = sdc::sdcHelper::getNameFromID(u.ID);
  boost::optional<string> id, storedPw, pubkey;
  sql << "SELECT id, pw, pubkey FROM users WHERE id = :id",
    into(id), into(storedPw), into(pubkey), use(name);

  if(!id.is_initialized())
    throw sdc::AuthenticationException("unknown ID");

  // TODO: proper password hashing
  if(pw != storedPw.get())
    throw sdc::AuthenticationException("invalid password");

  string providedPubkey(u.publicKey.begin(), u.publicKey.end());
  if(providedPubkey != "" && providedPubkey != pubkey.get())
    throw sdc::AuthenticationException("public key can't change");

  // TODO: check callback (call echo) provided by identity before logging in

  // don't log passwords!
  cout << "Logging in " << u.ID << endl;
  sdc::User user;
  user.ID = u.ID;

  // make sure the pubkey from registering is used
  user.publicKey = sdc::ByteSeq(pubkey.get().begin(), pubkey.get().end());
  // TODO: make sure session created here doesn't leak
  auto proxy = server->exposeObject(new SessionImpl(user));
  return sdc::SessionIPrx::checkedCast(proxy);
}
