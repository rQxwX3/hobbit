#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <occurrence.hpp>

namespace test::mods {
using hbt::mods::Occurrence, hbt::mods::DateTime, hbt::mods::Interval,
    hbt::mods::Week;

TEST(OccurrenceTest, WeekdayRecurrentEnforcesWeekInterval) {
    EXPECT_THROW(Occurrence(DateTime::today(),
                            Week{{DateTime::weekday_t::FRIDAY}},
                            Interval::days(1)),
                 std::invalid_argument);
}

TEST(OccurrenceTest, WeekdayRecurrentEnforcesNonEmptyWeek) {
    EXPECT_THROW(Occurrence(DateTime::today(),
                            Week(std::vector<DateTime::weekday_t>{}),
                            Interval::days(1)),
                 std::invalid_argument);
}

TEST(OccurrenceTest, WeekdayRecurrentEnforcesDateListedInWeekdaysList) {
    auto datetime{DateTime{
        std::chrono::year{2026},
        std::chrono::month{4},
        std::chrono::day{16},
    }}; // Thursday

    EXPECT_THROW(Occurrence(DateTime::today(),
                            Week{{DateTime::weekday_t::FRIDAY}},
                            Interval::days(1)),
                 std::invalid_argument);
}

TEST(OccurrenceTest, NonRecurrentGetterFunctions) {
    auto now{DateTime::now()};
    auto occurrence{Occurrence{now}};

    EXPECT_FALSE(occurrence.getInterval().has_value());
    EXPECT_FALSE(occurrence.isRecurrent());

    EXPECT_EQ(occurrence.getDateTime(), now);
    // EXPECT_EQ(occurrence.getRecurrenceModel(), Occurrence::NonRecurrent{});

    EXPECT_EQ(occurrence.getWeekday(), now.getWeekday());
}

TEST(OccurrenceTest, IntervalRecurrentGetterFunctions) {
    auto now{DateTime::now()};
    auto occurrence{Occurrence{now, Interval::months(2)}};

    ASSERT_TRUE(occurrence.getInterval().has_value());
    EXPECT_EQ(occurrence.getInterval(), Interval::months(2));
    EXPECT_TRUE(occurrence.isRecurrent());

    EXPECT_EQ(occurrence.getDateTime(), now);
    // EXPECT_EQ(occurrence.getRecurrenceModel(),
    //           Occurrence::IntervalRecurrent{Interval::months(2)});

    EXPECT_EQ(occurrence.getWeekday(), now.getWeekday());
}

TEST(OccurrenceTest, WeekdayRecurrentGetterFunctions) {
    auto now{DateTime::now()};
    auto occurrence{
        Occurrence{now, Week{{now.getWeekday(), DateTime::weekday_t::SUNDAY}},
                   Interval::weeks(2)}};

    ASSERT_TRUE(occurrence.getInterval().has_value());
    EXPECT_EQ(occurrence.getInterval(), Interval::weeks(2));
    EXPECT_TRUE(occurrence.isRecurrent());

    EXPECT_EQ(occurrence.getDateTime(), now);
    // EXPECT_EQ(
    //     occurrence.getRecurrenceModel(),
    //     Occurrence::WeekdayRecurrent(
    //         DateTime::Week{{DateTime::weekday_t::SATURDAY,
    //         DateTime::weekday_t::SUNDAY}}, Interval::weeks(2)));

    EXPECT_EQ(occurrence.getWeekday(), now.getWeekday());
}

TEST(OccurrenceTest, NonRecurrentToFromJSON) {
    auto now{DateTime::now()};
    auto original{Occurrence{now}};
    auto json = original.toJSON();
    ASSERT_EQ(std::string(json.type_name()), "object");

    EXPECT_EQ(json["datetime"], original.getDateTime().toISO8601String());
    EXPECT_EQ(json["recurrence_model"],
              original.recurrenceModelToJSON(original.getRecurrenceModel()));

    auto restored{Occurrence::fromJSON(json)};
    ASSERT_TRUE(restored.has_value());
    // TODO: json with time too
    // EXPECT_EQ(restored.value().getDateTime(), original.getDateTime());
    EXPECT_EQ(restored.value().getWeekday(), original.getWeekday());
}

TEST(OccurrenceTest, IntervalRecurrentToFromJSON) {
    auto now{DateTime::now()};
    auto original{
        Occurrence{now, Week{{now.getWeekday(), DateTime::weekday_t::SATURDAY}},
                   Interval::weeks(1)}};
    auto json = original.toJSON();
    ASSERT_EQ(std::string(json.type_name()), "object");

    EXPECT_EQ(json["datetime"], original.getDateTime().toISO8601String());

    auto restored{Occurrence::fromJSON(json)};
    ASSERT_TRUE(restored.has_value());
    EXPECT_EQ(restored.value().getDateTime(), original.getDateTime());
    EXPECT_EQ(restored.value().getWeekday(), original.getWeekday());
    // EXPECT_EQ(restored->getRecurrenceModel(), original.getRecurrenceModel());

    ASSERT_TRUE(restored->getInterval().has_value());
    ASSERT_TRUE(original.getInterval().has_value());
    EXPECT_EQ(original.getInterval().value(), restored->getInterval().value());
}

TEST(OccurrenceTest, WeekdayRecurrentToFromJSON) {
    auto now{DateTime::now()};
    auto original{
        Occurrence{now, Week{{now.getWeekday(), DateTime::weekday_t::THURSDAY}},
                   Interval::weeks(1)}};
    auto json = original.toJSON();
    ASSERT_EQ(std::string(json.type_name()), "object");

    EXPECT_EQ(json["datetime"], original.getDateTime().toISO8601String());

    auto restored{Occurrence::fromJSON(json)};
    ASSERT_TRUE(restored.has_value());
    EXPECT_EQ(restored.value().getDateTime(), original.getDateTime());
    EXPECT_EQ(restored.value().getWeekday(), original.getWeekday());
    // EXPECT_EQ(restored->getRecurrenceModel(), original.getRecurrenceModel());

    ASSERT_TRUE(restored->getInterval().has_value());
    ASSERT_TRUE(original.getInterval().has_value());
    EXPECT_EQ(original.getInterval().value(), restored->getInterval().value());
}

TEST(OccurrenceTest, NonRecurrentIsForDateFalseForFutureDate) {
    auto datetime{DateTime{
        std::chrono::year{2026},
        std::chrono::month{3},
        std::chrono::day{16},
    }};

    auto occurrence{Occurrence{datetime}};
    EXPECT_FALSE(occurrence.isForDate(datetime + Interval::days(1)));
    EXPECT_FALSE(occurrence.isForDate(datetime + Interval::weeks(1)));
    EXPECT_FALSE(occurrence.isForDate(datetime + Interval::months(1)));
}

TEST(OccurrenceTest, IntervalRecurrentIsForDateTrueForFutureDate) {
    auto datetime{DateTime{
        std::chrono::year{2026},
        std::chrono::month{3},
        std::chrono::day{16},
    }};

    auto occurrence1{Occurrence{datetime, Interval::weeks(1)}};
    EXPECT_TRUE(occurrence1.isForDate(datetime + Interval::weeks(1)));
    EXPECT_TRUE(occurrence1.isForDate(datetime + Interval::weeks(2)));
    EXPECT_TRUE(occurrence1.isForDate(datetime + Interval::days(7)));

    auto occurrence2{Occurrence{datetime, Interval::months(1)}};
    EXPECT_TRUE(occurrence2.isForDate(datetime + Interval::months(1)));
    EXPECT_TRUE(occurrence2.isForDate(datetime + Interval::months(2)));
    EXPECT_TRUE(occurrence2.isForDate(datetime + Interval::months(12)));

    auto occurrence3{Occurrence{datetime, Interval::years(1)}};
    EXPECT_TRUE(occurrence3.isForDate(datetime + Interval::years(1)));
    EXPECT_TRUE(occurrence3.isForDate(datetime + Interval::years(2)));
    EXPECT_TRUE(occurrence3.isForDate(datetime + Interval::months(12)));
}

TEST(OccurrenceTest, WeekdayRecurrentIsForDateTrueForFutureDate) {
    auto datetime{DateTime{
        std::chrono::year{2026},
        std::chrono::month{3},
        std::chrono::day{16},
    }};

    auto occurrence1{Occurrence{datetime, Week{{datetime.getWeekday()}},
                                Interval::weeks(1)}};
    EXPECT_TRUE(occurrence1.isForDate(datetime + Interval::weeks(1)));
    EXPECT_TRUE(occurrence1.isForDate(datetime + Interval::weeks(2)));
    EXPECT_TRUE(occurrence1.isForDate(datetime + Interval::days(7)));

    auto occurrence2{Occurrence{datetime, Week{{datetime.getWeekday()}},
                                Interval::weeks(4)}};
    EXPECT_TRUE(occurrence2.isForDate(datetime + Interval::weeks(4)));
    EXPECT_TRUE(occurrence2.isForDate(datetime + Interval::weeks(8)));
    EXPECT_TRUE(occurrence2.isForDate(datetime + Interval::weeks(12)));

    auto occurrence3{Occurrence{datetime, Week{{datetime.getWeekday()}},
                                Interval::weeks(56)}};
    EXPECT_TRUE(occurrence3.isForDate(datetime + Interval::weeks(56)));
    EXPECT_TRUE(occurrence3.isForDate(datetime + Interval::weeks(112)));
    EXPECT_TRUE(occurrence3.isForDate(datetime + Interval::weeks(168)));
}

TEST(OccurrenceTest, NonRecurrentIsForDateTrueForSelf) {
    auto date1{DateTime{
        std::chrono::year{2026},
        std::chrono::month{3},
        std::chrono::day{16},
    }};

    auto occurrence{Occurrence{date1}};
    EXPECT_TRUE(occurrence.isForDate(date1));
}

TEST(OccurrenceTest, IntervalRecurrentIsForDateTrueForSelf) {
    auto date1{DateTime{
        std::chrono::year{2026},
        std::chrono::month{3},
        std::chrono::day{16},
    }};

    auto occurrence{Occurrence{date1, Interval::days(7)}};
    EXPECT_TRUE(occurrence.isForDate(date1));
}

TEST(OccurrenceTest, WeekdayRecurrentIsForDateTrueForSelf) {
    auto date1{DateTime{
        std::chrono::year{2026},
        std::chrono::month{3},
        std::chrono::day{16},
    }};

    auto occurrence{Occurrence{date1, Week{{DateTime::weekday_t::TUESDAY}},
                               Interval::weeks(1)}};
    EXPECT_TRUE(occurrence.isForDate(date1));
}

TEST(OccurrenceTest, NonRecurrentIsForDateFalseForPastDate) {
    auto date1{DateTime{
        std::chrono::year{2026},
        std::chrono::month{3},
        std::chrono::day{23},
    }};

    auto date2{DateTime{
        std::chrono::year{2026},
        std::chrono::month{3},
        std::chrono::day{16},
    }};

    auto occurrence{Occurrence{date1}};

    EXPECT_FALSE(occurrence.isForDate(date2));
}

TEST(OccurrenceTest, IntervalRecurrentIsForDateFalseForPastDate) {
    auto date1{DateTime{
        std::chrono::year{2026},
        std::chrono::month{3},
        std::chrono::day{23},
    }};

    auto date2{DateTime{
        std::chrono::year{2026},
        std::chrono::month{3},
        std::chrono::day{16},
    }};

    auto occurrence{Occurrence{date1, Interval::days(7)}};

    EXPECT_FALSE(occurrence.isForDate(date2));
}

TEST(OccurrenceTest, WeekdayRecurrentIsForDateFalseForPastDate) {
    auto date1{DateTime{
        std::chrono::year{2026},
        std::chrono::month{3},
        std::chrono::day{23},
    }};

    auto date2{DateTime{
        std::chrono::year{2026},
        std::chrono::month{3},
        std::chrono::day{16},
    }};

    auto occurrence{
        Occurrence{date1, Week{{date1.getWeekday()}}, Interval::weeks(7)}};

    EXPECT_FALSE(occurrence.isForDate(date2));
}

TEST(OccurrenceTest, NonRecurrentIsForDateFalseForWrongDate) {
    auto date1{DateTime{
        std::chrono::year{2026},
        std::chrono::month{3},
        std::chrono::day{16},
    }};

    auto date2{DateTime{
        std::chrono::year{2026},
        std::chrono::month{3},
        std::chrono::day{17},
    }};

    auto occurrence{Occurrence{date1}};

    EXPECT_FALSE(occurrence.isForDate(date2));
}

TEST(OccurrenceTest, IntervalRecurrentIsForDateFalseForWrongDate) {
    auto date1{DateTime{
        std::chrono::year{2026},
        std::chrono::month{3},
        std::chrono::day{16},
    }};

    auto date2{DateTime{
        std::chrono::year{2026},
        std::chrono::month{3},
        std::chrono::day{17},
    }};

    auto occurrence{Occurrence{date1, Interval::days(7)}};

    EXPECT_FALSE(occurrence.isForDate(date2));
}

TEST(OccurrenceTest, WeekdayRecurrentIsForDateFalseForWrongDate) {
    auto date1{DateTime{
        std::chrono::year{2026},
        std::chrono::month{3},
        std::chrono::day{16},
    }};

    auto date2{DateTime{
        std::chrono::year{2026},
        std::chrono::month{3},
        std::chrono::day{17},
    }};

    auto occurrence{
        Occurrence{date1, Week{{date1.getWeekday()}}, Interval::weeks(7)}};

    EXPECT_FALSE(occurrence.isForDate(date2));
}
} // namespace test::mods
