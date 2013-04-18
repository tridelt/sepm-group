#include "gtest/gtest.h"

TEST(SanityTest, CanTest) {
  EXPECT_EQ(1, 1);
}

TEST(SanityTest, CanTestMore) {
  EXPECT_EQ(2, 2);
}

TEST(SanityTest, CStringEquality) {
  EXPECT_STREQ("hello", "hello");
}
