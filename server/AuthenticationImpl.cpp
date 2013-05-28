#include "AuthenticationImpl.h"
#include <iostream>
#include "DBPool.h"
#include "sdcHelper.h"
#include <boost/optional.hpp>
#include "SessionImpl.h"
#include "Logging.h"
#include "IceServerI.h"
#include "config.h"
#include "PwdHash.h"

using namespace std;
using namespace soci;

void AuthenticationImpl::registerUser(const sdc::User &u, const string &pw,
  const Ice::Current&) {
  // Be conservative in what you send, be liberal in what you accept
  string name = u.ID;
  if(sdc::sdcHelper::isValidID(u.ID)) {
     name = sdc::sdcHelper::getNameFromID(u.ID);
     if(sdc::sdcHelper::getServerFromID(u.ID) != Config::hostname())
       WARN("got register for different server: ", sdc::sdcHelper::getServerFromID(u.ID));
  } else {
    WARN("malformed ID ", u.ID);
  }

  session sql(server->getDBPool()->getPool());

  sql << "CREATE TABLE IF NOT EXISTS users(id text, pw text, pubkey text)";

  boost::optional<string> id;
  sql << "SELECT id FROM users WHERE id = :id",
    into(id), use(name);

  if(id.is_initialized()) {
    INFO("rejected register because ID already exists");
    throw sdc::AuthenticationException("ID already exists");
  }

  string pubkey(u.publicKey.begin(), u.publicKey.end()), hash;

  try {
    hash = genhash(pw, gensetting());
  }
  catch(...) {
    ERROR("internal failure when hashing");
    throw sdc::AuthenticationException("internal failure, please notify an admin");
  }

  sql << "INSERT INTO users(id, pw, pubkey) VALUES (:id, :pw, :pubkey)",
    use(name), use(hash), use(pubkey);

  // don't log passwords!
  INFO("Registered ", u.ID);
}


sdc::SessionIPrx AuthenticationImpl::login(const sdc::User &u, const string &pw,
    const Ice::Identity &callbackID, const Ice::Current &cur) {
  // Be conservative in what you send, be liberal in what you accept
  string name = u.ID;
  if(sdc::sdcHelper::isValidID(u.ID)) {
     name = sdc::sdcHelper::getNameFromID(u.ID);
     if(sdc::sdcHelper::getServerFromID(u.ID) != Config::hostname())
       WARN("got login for different server: ", sdc::sdcHelper::getServerFromID(u.ID));
  } else {
    WARN("malformed ID ", u.ID);
  }

  session sql(server->getDBPool()->getPool());

  sql << "CREATE TABLE IF NOT EXISTS users(id text, pw text, pubkey text)";

  boost::optional<string> id, storedPw, pubkey;
  sql << "SELECT id, pw, pubkey FROM users WHERE id = :id",
    into(id), into(storedPw), into(pubkey), use(name);

  if(!id.is_initialized()) {
    INFO("rejected login for unknown ID: ", name);
    throw sdc::AuthenticationException("unknown ID");
  }


  bool validpass = false;
  try {
    validpass = checkhash(pw, storedPw.get());
  } catch(...) {
    // check failure separately to avoid confusing users
    ERROR("internal failure when hashing");
    throw sdc::AuthenticationException("internal failure, please notify an admin");
  }

  if(!validpass) {
    INFO("rejected login for invalid password");
    throw sdc::AuthenticationException("invalid password");
  }

  string providedPubkey(u.publicKey.begin(), u.publicKey.end());
  if(providedPubkey != "" && providedPubkey != pubkey.get()) {
    INFO("rejected login for changed pubkey");
    throw sdc::AuthenticationException("public key can't change");
  }

  auto callback = server->callbackForID(callbackID, cur.con);
  if(callback->echo("42") != "42") {
    INFO("rejected login because callback doesn't work");
    throw sdc::AuthenticationException("echo not working");
  }

  // don't log passwords!
  INFO("Logging in ", u.ID);
  sdc::User user;
  user.ID = name;

  // make sure the pubkey from registering is used, not some other key
  user.publicKey = sdc::ByteSeq(pubkey.get().begin(), pubkey.get().end());

  // TODO: make sure session created here doesn't leak
  auto proxy = server->exposeObject(new SessionImpl(user, server, callback));
  return sdc::SessionIPrx::checkedCast(proxy);
}
