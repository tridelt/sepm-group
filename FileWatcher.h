#include <string>
#include <QtDeclarative/QDeclarativeView>
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
  void addWatch(QDeclarativeView *view, QString layout) {
    views[layout] = view;
  }
  void watch(string dir);
  void start(string dir);
  void stop();

public slots:
  void refreshView(QDeclarativeView* view, const QUrl &url);

private:
  QMap<QString, QDeclarativeView*> views;
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
