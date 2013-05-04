#include "AuthenticationImpl.h"
#include <iostream>

using namespace std;

void AuthenticationImpl::registerUser(const sdc::User &u, const string &pw,
  const Ice::Current&) {
  cout << "Registering " << u.ID << " with pw " << pw << endl;
  cout << "not implemented" << endl;
}

sdc::SessionIPrx AuthenticationImpl::login(const sdc::User &u, const string &pw,
    const Ice::Identity&, const Ice::Current&) {
  cout << "Logging in " << u.ID << " with pw " << pw << endl;
  cout << "login not implemented" << endl;
  throw sdc::AuthenticationException();
}
