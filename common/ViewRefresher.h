#include <string>
#include <QtDeclarative/QDeclarativeView>
#include <QObject>
#include <QMap>
#include "FileWatcher.h"

using namespace std;

class ViewRefresher : public QObject {
  Q_OBJECT

public:
  void addWatch(QDeclarativeView *view, QString layout) {
    views[layout] = view;
  }
  void fileChanged(string name, bool isDir, FileWatcher::FileEvent e);

public slots:
  void refreshView(QDeclarativeView* view, const QUrl &url);

private:
  QMap<QString, QDeclarativeView*> views;
};
