#include <string>
#include <QObject>
#include "NotifyPipe.h"
#include <QFuture>

using namespace std;

class FileWatcher : public QObject {
  Q_OBJECT

public:
  FileWatcher(string dir, function<void()> callback);
  ~FileWatcher();

private:
  void watch(string dir, function<void()> callback);

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
