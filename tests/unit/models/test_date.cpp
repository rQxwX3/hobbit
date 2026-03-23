#include <gtest/gtest.h>

#include <date.hpp>

namespace test::mods {
TEST(DateTest, ToFromYMDString) {
    using hbt::mods::Date;

    auto original{Date{}};

    auto ymdString{original.toYMDString()};

    auto restored{Date::fromYMDString(ymdString)};
    EXPECT_EQ(restored.getYear(), original.getYear());
    EXPECT_EQ(restored.getMonth(), original.getMonth());
    EXPECT_EQ(restored.getDay(), original.getDay());
}
} // namespace test::mods
