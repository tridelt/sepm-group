#include "gtest/gtest.h"

TEST(SanityTest, CanTest) {
  EXPECT_EQ(1, 1);
}

TEST(SanityTest, CStringEquality) {
  EXPECT_STREQ("hello", "helloo");
}
