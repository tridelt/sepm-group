#include "FileWatcher.h"
#include <sys/inotify.h>
#include <iostream>
#include <unistd.h>
#include <QtConcurrentRun>
#include <QtDeclarative/QDeclarativeEngine>

#define EVENT_SIZE  ( sizeof (struct inotify_event) )
#define BUF_LEN     ( 1024 * ( EVENT_SIZE + 16 ) )

void FileWatcher::watch(string dir, function<void()> callback) {
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
          // notify that some file changed
          callback();
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


FileWatcher::FileWatcher(string dir, function<void()> callback) :
  running(true), future(QtConcurrent::run(this, &FileWatcher::watch, dir, callback)) {
}


FileWatcher::~FileWatcher() {
  running = false;
  pipe.notify();
  future.waitForFinished();
}
