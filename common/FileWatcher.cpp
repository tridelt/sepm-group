#include "FileWatcher.h"
#include <sys/inotify.h>
#include <iostream>
#include <unistd.h>
#include <QtConcurrentRun>
#include <QtDeclarative/QDeclarativeEngine>

#define EVENT_SIZE  ( sizeof (struct inotify_event) )
#define BUF_LEN     ( 1024 * ( EVENT_SIZE + 16 ) )

void FileWatcher::watch(string dir) {
  qRegisterMetaType<QDeclarativeView*>("QDeclarativeView*");

  int fd = inotify_init();
  if(fd < 0)
    throw WatchException("failed to init inotify");

  int wd = inotify_add_watch( fd, dir.c_str(),
                         IN_MODIFY | IN_CREATE | IN_DELETE );

  char buffer[BUF_LEN];
  while(running) {
    fd_set descriptors;

    FD_ZERO( &descriptors);
    FD_SET( pipe.receiverFd(), &descriptors);  // pipe to signal end of execution
    FD_SET( fd, &descriptors);                 // fd from inotify
    int nfds = max(fd, pipe.receiverFd()) + 1;

    int retval = select ( nfds, &descriptors, NULL, NULL, NULL);

    if(retval == -1) {
      throw WatchException("select failed");
    } else if(retval) {
      if(FD_ISSET ( pipe.receiverFd(), &descriptors)) {
        // signal to stop watching
        break;
      } else if(FD_ISSET ( fd, &descriptors)) {
        int length = read( fd, buffer, BUF_LEN );
        if(length < 0) {
          throw WatchException("failed to read() events");
        } else {
          // refresh all views
          QMapIterator<QString, QDeclarativeView *> i(views);
          while(i.hasNext()) {
            i.next();
            QMetaObject::invokeMethod(this, "refreshView", Qt::QueuedConnection,
                                    Q_ARG(QDeclarativeView*, i.value()),
                                    Q_ARG(QUrl, QUrl(i.key())));
          }
        }
      } else {
        throw WatchException("unexpected fd");
      }
    } else { // timeout
      throw WatchException("select should never timeout");
    }
  }

  // cleanup
  inotify_rm_watch(fd, wd);
  close(fd);
}


void FileWatcher::start(string dir) {
  running = true;
  future = QtConcurrent::run(this, &FileWatcher::watch, dir);
}


void FileWatcher::stop() {
  running = false;
  pipe.notify();
  future.waitForFinished();
}

void FileWatcher::refreshView(QDeclarativeView* view, const QUrl &url) {
  // important: .qml is only reloaded if the cache is cleared first
  view->engine()->clearComponentCache();
  view->setSource(url);
  view->show();
}
