#include "SessionManager.h"
#include "SessionImpl.h"

SessionManager::SessionManager() {
}

void SessionManager::addSession(SessionImpl *s) {
  sessions.insert(pair<sessionlist::key_type, sessionlist::mapped_type>(s->getUser().ID, s));
}

pair<SessionManager::sessionlist::iterator, SessionManager::sessionlist::iterator>
  SessionManager::getSessions(const string &id) {
  return sessions.equal_range(id);
}

void SessionManager::removeSession(const string &id, SessionImpl *s) {
  auto sl = getSessions(id);
  for(auto iter = sl.first; iter != sl.second; ++iter) {
    if(iter->second == s) sessions.erase(iter);
  }
}
