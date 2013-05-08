#include "gtest/gtest.h"
#include "Logging.h"

class TestSink : public virtual LogSink {
public:
  TestSink(bool chatty, Severity l) : LogSink(chatty, l) {
    gotLines = 0;
    chatty = false;
  }
  void write(string s) {
    gotLines += 1;
    if(s.find("logging.cpp") != string::npos) {
      chatty = true;
    }
  }
  int gotLines;
  bool chatty;
};

TEST(Logging, ReceiveLog) {
  Logger myLogger;
  myLogger.clearSinks();
  TestSink sink(true, Severity::LVL_INFO);
  myLogger.addSink(&sink);

  myLogger.log<Severity::LVL_INFO>(__FILE__, __LINE__).print("test");
  ASSERT_EQ(1, sink.gotLines);
}


TEST(Logging, HideLowerLogs) {
  // sinks should only receive messages >= to their severity level
  Logger myLogger;
  myLogger.clearSinks();
  TestSink sink1(true, Severity::LVL_INFO);
  TestSink sink2(true, Severity::LVL_WARN);
  TestSink sink3(true, Severity::LVL_ERROR);
  myLogger.addSink(&sink1);
  myLogger.addSink(&sink2);
  myLogger.addSink(&sink3);

  myLogger.log<Severity::LVL_INFO>(__FILE__, __LINE__).print("test");
  ASSERT_EQ(1, sink1.gotLines);
  ASSERT_EQ(0, sink2.gotLines);
  ASSERT_EQ(0, sink3.gotLines);

  myLogger.log<Severity::LVL_ERROR>(__FILE__, __LINE__).print("test");
  ASSERT_EQ(2, sink1.gotLines);
  ASSERT_EQ(1, sink2.gotLines);
  ASSERT_EQ(1, sink3.gotLines);
}

TEST(Logging, ChattyMessages) {
  Logger myLogger;
  myLogger.clearSinks();
  TestSink sink(false, Severity::LVL_INFO);
  TestSink sink_chatty(true, Severity::LVL_INFO);
  myLogger.addSink(&sink);
  myLogger.addSink(&sink_chatty);

  myLogger.log<Severity::LVL_INFO>(__FILE__, __LINE__).print("test");
  ASSERT_EQ(1, sink.gotLines);
  ASSERT_FALSE(sink.chatty);
  ASSERT_EQ(1, sink_chatty.gotLines);
  ASSERT_TRUE(sink_chatty.chatty);
}
