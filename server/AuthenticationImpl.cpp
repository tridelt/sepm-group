#include "AuthenticationImpl.h"
#include <iostream>
#include "DBPool.h"
#include "sdcHelper.h"
#include <boost/optional.hpp>
#include "SessionImpl.h"
#include "Logging.h"
#include "IceServer.h"
#include "PwdHash.h"

using namespace std;
using namespace soci;

void AuthenticationImpl::registerUser(const sdc::User &u, const string &pw,
  const Ice::Current&) {
  // Be conservative in what you send, be liberal in what you accept
  string name = u.ID;
  if(sdc::sdcHelper::isValidID(u.ID))
     name = sdc::sdcHelper::getNameFromID(u.ID);

  session sql(db_pool->getPool());

  sql << "CREATE TABLE IF NOT EXISTS users(id text, pw text, salt text, pubkey text)";

  boost::optional<string> id;
  sql << "SELECT id FROM users WHERE id = :id",
    into(id), use(name);

  if(id.is_initialized())
    throw sdc::AuthenticationException("ID already exists");

  string pubkey(u.publicKey.begin(), u.publicKey.end()), salt(gensalt()), hash;

  try {
    hash = makehash(pw, salt);
  }
  catch(...) {
    throw sdc::AuthenticationException("can't register right now");
  }

  sql << "INSERT INTO users(id, pw, salt, pubkey) VALUES (:id, :pw, :salt, :pubkey)",
    use(name), use(hash), use(salt), use(pubkey);

  // don't log passwords!
  INFO("Registered ", u.ID);
}

sdc::SessionIPrx AuthenticationImpl::login(const sdc::User &u, const string &pw,
    const Ice::Identity &callbackID, const Ice::Current &cur) {
  // Be conservative in what you send, be liberal in what you accept
  string name = u.ID;
  if(sdc::sdcHelper::isValidID(u.ID))
     name = sdc::sdcHelper::getNameFromID(u.ID);

  session sql(db_pool->getPool());

  sql << "CREATE TABLE IF NOT EXISTS users(id text, pw text, salt text, pubkey text)";

  boost::optional<string> id, storedPw, salt, pubkey;
  sql << "SELECT id, pw, salt, pubkey FROM users WHERE id = :id",
    into(id), into(storedPw), into(salt), into(pubkey), use(name);

  if(!id.is_initialized())
    throw sdc::AuthenticationException("unknown ID");

  bool validpass = false;
  try {
    validpass = checkhash(pw, storedPw.get(), salt.get());
  }
  catch(...) {
    // check failure separately to avoid confusing users
    throw sdc::AuthenticationException("internal failure");
  }

  if(!validpass)
    throw sdc::AuthenticationException("invalid password");

  string providedPubkey(u.publicKey.begin(), u.publicKey.end());
  if(providedPubkey != "" && providedPubkey != pubkey.get())
    throw sdc::AuthenticationException("public key can't change");


  auto callback = server->callbackForID(callbackID, cur.con);
  if(callback->echo("42") != "42")
    throw sdc::AuthenticationException("echo not working");

  // don't log passwords!
  INFO("Logging in ", u.ID);
  sdc::User user;
  user.ID = name;

  // make sure the pubkey from registering is used, not some other key
  user.publicKey = sdc::ByteSeq(pubkey.get().begin(), pubkey.get().end());

  // TODO: make sure session created here doesn't leak
  auto proxy = server->exposeObject(new SessionImpl(user, db_pool));
  return sdc::SessionIPrx::checkedCast(proxy);
}
