#include "gtest/gtest.h"

TEST(SanityTest, CanTest) {
  ASSERT_EQ(1, 1);
}

TEST(SanityTest, CStringEquality) {
  ASSERT_STREQ("hello", "hello");
}
