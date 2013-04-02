#include "gtest/gtest.h"
#include "NotifyPipe.h"

TEST(NotifyPipe, CanConstruct) {
  NotifyPipe p;
}

TEST(NotifyPipe, CanNotify) {
  NotifyPipe p;
  p.notify();

  char buf[20];
  EXPECT_GT(read(p.receiverFd(), buf, 20), 0);
}
