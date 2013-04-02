#include "gtest/gtest.h"

TEST(SanityTest, CanTest) {
  EXPECT_EQ(1, 2);
}

TEST(SanityTest, CStringEquality) {
  EXPECT_STREQ("hello", "hello");
}
