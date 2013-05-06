#include "AuthenticationImpl.h"
#include <iostream>
#include "DBPool.h"
#include "sdcHelper.h"
#include <boost/optional.hpp>
#include "SessionImpl.h"
#include "Logging.h"

using namespace std;
using namespace soci;

void AuthenticationImpl::registerUser(const sdc::User &u, const string &pw,
  const Ice::Current&) {
  // Be conservative in what you send, be liberal in what you accept
  string name = u.ID;
  if(sdc::sdcHelper::isValidID(u.ID))
     name = sdc::sdcHelper::getNameFromID(u.ID);

  session sql(DBPool::i()->getPool());

  sql << "CREATE TABLE IF NOT EXISTS users(id text, pw text, pubkey text)";

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
  INFO("Registered ", u.ID);
}

sdc::SessionIPrx AuthenticationImpl::login(const sdc::User &u, const string &pw,
    const Ice::Identity&, const Ice::Current&) {
  // Be conservative in what you send, be liberal in what you accept
  string name = u.ID;
  if(sdc::sdcHelper::isValidID(u.ID))
     name = sdc::sdcHelper::getNameFromID(u.ID);

  session sql(DBPool::i()->getPool());

  sql << "CREATE TABLE IF NOT EXISTS users(id text, pw text, pubkey text)";

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

  // TODO: check callback provided by identity before logging in (call echo)

  // don't log passwords!
  INFO("Logging in ", u.ID);
  sdc::User user;
  user.ID = name;

  // make sure the pubkey from registering is used, not some other key
  user.publicKey = sdc::ByteSeq(pubkey.get().begin(), pubkey.get().end());

  // TODO: make sure session created here doesn't leak
  auto proxy = server->exposeObject(new SessionImpl(user));
  return sdc::SessionIPrx::checkedCast(proxy);
}
