#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <occurrence.hpp>

namespace test::mods {
using hbt::mods::Occurrence, hbt::mods::Date, hbt::mods::Interval,
    hbt::mods::Week;

TEST(OccurrenceTest, WeekdayRecurrentEnforcesWeekInterval) {
    EXPECT_THROW(Occurrence(Date::today(), Week{{Date::weekday_t::FRIDAY}},
                            Interval::days(1)),
                 std::invalid_argument);
}

TEST(OccurrenceTest, WeekdayRecurrentEnforcesNonEmptyWeek) {
    EXPECT_THROW(Occurrence(Date::today(), Week(std::vector<Date::weekday_t>{}),
                            Interval::days(1)),
                 std::invalid_argument);
}

TEST(OccurrenceTest, WeekdayRecurrentEnforcesDateListedInWeekdaysList) {
    auto date{Date{
        std::chrono::year{2026},
        std::chrono::month{4},
        std::chrono::day{16},
    }}; // Thursday

    EXPECT_THROW(Occurrence(Date::today(), Week{{Date::weekday_t::FRIDAY}},
                            Interval::days(1)),
                 std::invalid_argument);
}

TEST(OccurrenceTest, NonRecurrentGetterFunctions) {
    auto occurrence{Occurrence{}};

    EXPECT_FALSE(occurrence.getInterval().has_value());
    EXPECT_FALSE(occurrence.isRecurrent());

    EXPECT_EQ(occurrence.getDate(), Date::today());
    // EXPECT_EQ(occurrence.getRecurrenceModel(), Occurrence::NonRecurrent{});

    EXPECT_EQ(occurrence.getWeekday(), Date::today().getWeekday());
}

TEST(OccurrenceTest, IntervalRecurrentGetterFunctions) {
    auto occurrence{Occurrence{Date::today(), Interval::months(2)}};

    ASSERT_TRUE(occurrence.getInterval().has_value());
    EXPECT_EQ(occurrence.getInterval(), Interval::months(2));
    EXPECT_TRUE(occurrence.isRecurrent());

    EXPECT_EQ(occurrence.getDate(), Date::today());
    // EXPECT_EQ(occurrence.getRecurrenceModel(),
    //           Occurrence::IntervalRecurrent{Interval::months(2)});

    EXPECT_EQ(occurrence.getWeekday(), Date::today().getWeekday());
}

TEST(OccurrenceTest, WeekdayRecurrentGetterFunctions) {
    auto occurrence{
        Occurrence{Date::today(),
                   Week{{Date::today().getWeekday(), Date::weekday_t::SUNDAY}},
                   Interval::weeks(2)}};

    ASSERT_TRUE(occurrence.getInterval().has_value());
    EXPECT_EQ(occurrence.getInterval(), Interval::weeks(2));
    EXPECT_TRUE(occurrence.isRecurrent());

    EXPECT_EQ(occurrence.getDate(), Date::today());
    // EXPECT_EQ(
    //     occurrence.getRecurrenceModel(),
    //     Occurrence::WeekdayRecurrent(
    //         Date::Week{{Date::weekday_t::SATURDAY, Date::weekday_t::SUNDAY}},
    //         Interval::weeks(2)));

    EXPECT_EQ(occurrence.getWeekday(), Date::today().getWeekday());
}

TEST(OccurrenceTest, NonRecurrentToFromJSON) {
    auto original{Occurrence{}};
    auto json = original.toJSON();
    ASSERT_EQ(std::string(json.type_name()), "object");

    EXPECT_EQ(json["date"], original.getDate().toISO8601String());
    EXPECT_EQ(json["recurrence_model"],
              original.recurrenceModelToJSON(original.getRecurrenceModel()));

    auto restored{Occurrence::fromJSON(json)};
    ASSERT_TRUE(restored.has_value());
    EXPECT_EQ(restored.value().getDate(), original.getDate());
    EXPECT_EQ(restored.value().getWeekday(), original.getWeekday());
}

TEST(OccurrenceTest, IntervalRecurrentToFromJSON) {
    auto original{
        Occurrence{Date::today(),
                   Week{{Date::weekday_t::FRIDAY, Date::weekday_t::SATURDAY}},
                   Interval::weeks(1)}};
    auto json = original.toJSON();
    ASSERT_EQ(std::string(json.type_name()), "object");

    EXPECT_EQ(json["date"], original.getDate().toISO8601String());

    auto restored{Occurrence::fromJSON(json)};
    ASSERT_TRUE(restored.has_value());
    EXPECT_EQ(restored.value().getDate(), original.getDate());
    EXPECT_EQ(restored.value().getWeekday(), original.getWeekday());
    // EXPECT_EQ(restored->getRecurrenceModel(), original.getRecurrenceModel());

    ASSERT_TRUE(restored->getInterval().has_value());
    ASSERT_TRUE(original.getInterval().has_value());
    EXPECT_EQ(original.getInterval().value(), restored->getInterval().value());
}

TEST(OccurrenceTest, WeekdayRecurrentToFromJSON) {
    auto original{Occurrence{
        Date::today(),
        Week{{Date::today().getWeekday(), Date::weekday_t::THURSDAY}},
        Interval::weeks(1)}};
    auto json = original.toJSON();
    ASSERT_EQ(std::string(json.type_name()), "object");

    EXPECT_EQ(json["date"], original.getDate().toISO8601String());

    auto restored{Occurrence::fromJSON(json)};
    ASSERT_TRUE(restored.has_value());
    EXPECT_EQ(restored.value().getDate(), original.getDate());
    EXPECT_EQ(restored.value().getWeekday(), original.getWeekday());
    // EXPECT_EQ(restored->getRecurrenceModel(), original.getRecurrenceModel());

    ASSERT_TRUE(restored->getInterval().has_value());
    ASSERT_TRUE(original.getInterval().has_value());
    EXPECT_EQ(original.getInterval().value(), restored->getInterval().value());
}

TEST(OccurrenceTest, NonRecurrentIsForDateFalseForFutureDate) {
    auto date{Date{
        std::chrono::year{2026},
        std::chrono::month{3},
        std::chrono::day{16},
    }};

    auto occurrence{Occurrence{date}};
    EXPECT_FALSE(occurrence.isForDate(date + Interval::days(1)));
    EXPECT_FALSE(occurrence.isForDate(date + Interval::weeks(1)));
    EXPECT_FALSE(occurrence.isForDate(date + Interval::months(1)));
}

TEST(OccurrenceTest, IntervalRecurrentIsForDateTrueForFutureDate) {
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

TEST(OccurrenceTest, WeekdayRecurrentIsForDateTrueForFutureDate) {
    auto date{Date{
        std::chrono::year{2026},
        std::chrono::month{3},
        std::chrono::day{16},
    }};

    auto occurrence1{
        Occurrence{date, Week{{date.getWeekday()}}, Interval::weeks(1)}};
    EXPECT_TRUE(occurrence1.isForDate(date + Interval::weeks(1)));
    EXPECT_TRUE(occurrence1.isForDate(date + Interval::weeks(2)));
    EXPECT_TRUE(occurrence1.isForDate(date + Interval::days(7)));

    auto occurrence2{
        Occurrence{date, Week{{date.getWeekday()}}, Interval::weeks(4)}};
    EXPECT_TRUE(occurrence2.isForDate(date + Interval::weeks(4)));
    EXPECT_TRUE(occurrence2.isForDate(date + Interval::weeks(8)));
    EXPECT_TRUE(occurrence2.isForDate(date + Interval::weeks(12)));

    auto occurrence3{
        Occurrence{date, Week{{date.getWeekday()}}, Interval::weeks(56)}};
    EXPECT_TRUE(occurrence3.isForDate(date + Interval::weeks(56)));
    EXPECT_TRUE(occurrence3.isForDate(date + Interval::weeks(112)));
    EXPECT_TRUE(occurrence3.isForDate(date + Interval::weeks(168)));
}

TEST(OccurrenceTest, NonRecurrentIsForDateTrueForSelf) {
    auto date1{Date{
        std::chrono::year{2026},
        std::chrono::month{3},
        std::chrono::day{16},
    }};

    auto occurrence{Occurrence{date1}};
    EXPECT_TRUE(occurrence.isForDate(date1));
}

TEST(OccurrenceTest, IntervalRecurrentIsForDateTrueForSelf) {
    auto date1{Date{
        std::chrono::year{2026},
        std::chrono::month{3},
        std::chrono::day{16},
    }};

    auto occurrence{Occurrence{date1, Interval::days(7)}};
    EXPECT_TRUE(occurrence.isForDate(date1));
}

TEST(OccurrenceTest, WeekdayRecurrentIsForDateTrueForSelf) {
    auto date1{Date{
        std::chrono::year{2026},
        std::chrono::month{3},
        std::chrono::day{16},
    }};

    auto occurrence{Occurrence{date1, Week{{Date::weekday_t::TUESDAY}},
                               Interval::weeks(1)}};
    EXPECT_TRUE(occurrence.isForDate(date1));
}

TEST(OccurrenceTest, NonRecurrentIsForDateFalseForPastDate) {
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

    auto occurrence{Occurrence{date1}};

    EXPECT_FALSE(occurrence.isForDate(date2));
}

TEST(OccurrenceTest, IntervalRecurrentIsForDateFalseForPastDate) {
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

TEST(OccurrenceTest, WeekdayRecurrentIsForDateFalseForPastDate) {
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

    auto occurrence{
        Occurrence{date1, Week{{date1.getWeekday()}}, Interval::weeks(7)}};

    EXPECT_FALSE(occurrence.isForDate(date2));
}

TEST(OccurrenceTest, NonRecurrentIsForDateFalseForWrongDate) {
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

    auto occurrence{Occurrence{date1}};

    EXPECT_FALSE(occurrence.isForDate(date2));
}

TEST(OccurrenceTest, IntervalRecurrentIsForDateFalseForWrongDate) {
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

TEST(OccurrenceTest, WeekdayRecurrentIsForDateFalseForWrongDate) {
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

    auto occurrence{
        Occurrence{date1, Week{{date1.getWeekday()}}, Interval::weeks(7)}};

    EXPECT_FALSE(occurrence.isForDate(date2));
}
} // namespace test::mods
