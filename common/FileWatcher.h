#ifndef FILEWATCHER_H
#define FILEWATCHER_H

#include <string>
#include <QObject>
#include "NotifyPipe.h"
#include <QFuture>

using namespace std;



class FileWatcher : public QObject {
  Q_OBJECT

public:
  enum FileEvent {
    CREATE,
    MODIFY,
    DELETE
  };

  FileWatcher(string dir, function<void(string, bool, FileEvent)> callback);
  ~FileWatcher();

private:
  void watch(string dir, function<void(string, bool, FileEvent)> callback);

  bool running;
  NotifyPipe pipe;
  QFuture<void> future;
};

class WatchException : exception {
public:
  WatchException(string reason) : reason(reason) { }
  const char* what() const throw() {
    return reason.c_str();
  }
  ~WatchException() throw() { }
private:
  string reason;
};

#endif
