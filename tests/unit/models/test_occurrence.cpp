#include <gtest/gtest.h>

#include <occurrence.hpp>

namespace test::mods {
using hbt::mods::Occurrence, hbt::mods::Date, hbt::mods::Interval;

TEST(OccurrenceTest, ToFromJSON) {
    auto original{Occurrence{}};
    auto json = original.toJSON();
    ASSERT_EQ(std::string(json.type_name()), "object");

    EXPECT_EQ(json["date"], original.getDate().toISO8601String());

    auto restored{Occurrence::fromJSON(json)};
    ASSERT_TRUE(restored.has_value());
    EXPECT_EQ(restored.value().getDate().toISO8601String(),
              original.getDate().toISO8601String());
    EXPECT_EQ(restored.value().getWeekday(), original.getWeekday());
}

TEST(OccurrenceTest, IsForDateTrueForFutureDate) {
    auto date{Date{
        std::chrono::year{2026},
        std::chrono::month{3},
        std::chrono::day{16},
    }};

    auto occurrence1{Occurrence{date, Interval::weeks(1)}};
    EXPECT_TRUE(occurrence1.isForDate(date + Interval::weeks(1)));
    EXPECT_TRUE(occurrence1.isForDate(date + Interval::weeks(2)));
    EXPECT_TRUE(occurrence1.isForDate(date + Interval::days(7)));

    auto occurrence2{Occurrence{date, Interval::months(1)}};
    EXPECT_TRUE(occurrence2.isForDate(date + Interval::months(1)));
    EXPECT_TRUE(occurrence2.isForDate(date + Interval::months(2)));
    EXPECT_TRUE(occurrence2.isForDate(date + Interval::months(12)));

    auto occurrence3{Occurrence{date, Interval::years(1)}};
    EXPECT_TRUE(occurrence3.isForDate(date + Interval::years(1)));
    EXPECT_TRUE(occurrence3.isForDate(date + Interval::years(2)));
    EXPECT_TRUE(occurrence3.isForDate(date + Interval::months(12)));
}

TEST(OccurrenceTest, IsForDateTrueForSelf) {
    auto date1{Date{
        std::chrono::year{2026},
        std::chrono::month{3},
        std::chrono::day{16},
    }};

    auto occurrence{Occurrence{date1, Interval::days(7)}};
    EXPECT_TRUE(occurrence.isForDate(date1));
}

TEST(OccurrenceTest, IsForDateFalseForPastDate) {
    auto date1{Date{
        std::chrono::year{2026},
        std::chrono::month{3},
        std::chrono::day{23},
    }};

    auto date2{Date{
        std::chrono::year{2026},
        std::chrono::month{3},
        std::chrono::day{16},
    }};

    auto occurrence{Occurrence{date1, Interval::days(7)}};

    EXPECT_FALSE(occurrence.isForDate(date2));
}

TEST(OccurrenceTest, IsForDateFalseForWrongDate) {
    auto date1{Date{
        std::chrono::year{2026},
        std::chrono::month{3},
        std::chrono::day{16},
    }};

    auto date2{Date{
        std::chrono::year{2026},
        std::chrono::month{3},
        std::chrono::day{17},
    }};

    auto occurrence{Occurrence{date1, Interval::days(7)}};

    EXPECT_FALSE(occurrence.isForDate(date2));
}
} // namespace test::mods
