#ifndef SESSION_MANAGER_H
#define SESSION_MANAGER_H

#include "SecureDistributedChat.h"
#include <boost/shared_ptr.hpp>
#include "SessionImpl.h"

using namespace std;

/**
 * Manages sessions on this server. Note that a user may have multiple
 * concurrent sessions.
*/
class SessionManager {
  public:
    typedef multimap<string, shared_ptr<SessionImpl>> sessionlist;
    SessionManager();
    void addSession(shared_ptr<SessionImpl> s);
    pair<sessionlist::iterator, sessionlist::iterator> getSessions(const string &id);
    void removeSession(const string &id, shared_ptr<SessionImpl> s);
  private:
    sessionlist sessions;
};

#define userCallback(u, callback) {\
  auto sl = getSessions(u);\
  for(auto iter = sl.first; iter != sl.second; ++iter) {\
    iter->second->getCallback()->callback;\
  }\
}

#endif
