#include <string>
#include <QObject>
#include "NotifyPipe.h"
#include <QFuture>

using namespace std;

class FileWatcher : public QObject {
  Q_OBJECT

public:
  ~FileWatcher() {
    stop();
  }
  void start(string dir, function<void()> callback);
  void stop();

private:
  void watch(string dir, function<void()> callback);

  NotifyPipe pipe;
  QFuture<void> future;
  bool running;
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
