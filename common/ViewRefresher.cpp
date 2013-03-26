#include "ViewRefresher.h"
#include <QtDeclarative/QDeclarativeEngine>

void ViewRefresher::refreshView(QDeclarativeView* view, const QUrl &url) {
  // important: .qml is only reloaded if the cache is cleared first
  view->engine()->clearComponentCache();
  view->setSource(url);
  view->show();
}

void ViewRefresher::fileChanged(string, bool, FileWatcher::FileEvent) {
  qRegisterMetaType<QDeclarativeView*>("QDeclarativeView*");
  // refresh all views
  QMapIterator<QString, QDeclarativeView *> i(views);
  while(i.hasNext()) {
    i.next();
    QMetaObject::invokeMethod(this, "refreshView", Qt::QueuedConnection,
                            Q_ARG(QDeclarativeView*, i.value()),
                            Q_ARG(QUrl, QUrl(i.key())));
  }
}
