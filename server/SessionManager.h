#ifndef SESSION_MANAGER_H
#define SESSION_MANAGER_H

#include "SecureDistributedChat.h"
#include <boost/shared_ptr.hpp>

class SessionImpl;

using namespace std;

/**
 * Manages sessions on this server. Note that a user may have multiple
 * concurrent sessions.
*/
class SessionManager {
  public:
    typedef multimap<string, SessionImpl*> sessionlist;
    SessionManager();
    void addSession(SessionImpl *s);
    pair<sessionlist::iterator, sessionlist::iterator> getSessions(const string &id);
    void removeSession(const string &id, SessionImpl *s);
  private:
    sessionlist sessions;
};

#define userCallback(srv, u, callback) {\
  auto sl = srv->getSessions()->getSessions(u.ID);\
  for(auto iter = sl.first; iter != sl.second; ++iter) {\
    iter->second->getCallback()->callback;\
  }\
}

#endif
