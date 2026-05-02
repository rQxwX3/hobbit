#include <ymd.hpp>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace test::mods {
using hbt::mods::YMD;

TEST(YMDTest, ConstructsValidYMD) {
    EXPECT_NO_THROW(YMD(2026, 5, 2));

    EXPECT_NO_THROW(YMD(100, 5, 2));

    EXPECT_NO_THROW(YMD(0, 1, 1));

    EXPECT_NO_THROW(YMD(-1, 1, 1));
}

TEST(YMDTest, ThrowsOnInvalidParameters) {
    EXPECT_THROW(YMD(2026, 2, 29), std::invalid_argument); // not a leap year

    EXPECT_THROW(YMD(2026, 13, 1), std::invalid_argument); // invalid month

    EXPECT_THROW(YMD(2026, 1, 32), std::invalid_argument); // january has 31 day

    EXPECT_THROW(YMD(2026, 13, 32),
                 std::invalid_argument); // invalid month & day

    EXPECT_THROW(YMD(2026, 0, 0),
                 std::invalid_argument); // invalid month & day

    EXPECT_THROW(YMD(2026, -1, -1),
                 std::invalid_argument); // invalid month & day
}
} // namespace test::mods
