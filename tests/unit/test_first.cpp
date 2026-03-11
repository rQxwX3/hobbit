#include <gtest/gtest.h>

TEST(FirstTest, SanityCheck) {
    EXPECT_EQ(1 + 1, 2);
    EXPECT_TRUE(true);
    EXPECT_FALSE(false);
}
