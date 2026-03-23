#include <gtest/gtest.h>

#include <occurrence.hpp>

namespace test::mods {
TEST(OccurrenceTest, ToFromJSON) {
    using hbt::mods::Occurrence;

    auto original{Occurrence{}};
    auto json = original.toJSON();
    ASSERT_EQ(std::string(json.type_name()), "object");

    EXPECT_EQ(json["date"], original.getDate().toYMDString());

    auto restored{Occurrence::fromJSON(json)};
    ASSERT_TRUE(restored.has_value());
    EXPECT_EQ(restored.value().getDate().toYMDString(),
              original.getDate().toYMDString());
    EXPECT_EQ(restored.value().getWeekday(), original.getWeekday());
}

// TEST(OccurrenceTest, IsForDateTrueForFutureDate) {
//     using hbt::mods::Occurrence, hbt::mods::Date;
//
//     auto date{Date{
//         std::chrono::year{2026},
//         std::chrono::month{3},
//         std::chrono::day{16},
//     }};

// auto occurrence1{Occurrence{date, std::chrono::weeks{1}}};
// EXPECT_TRUE(occurrence1.isForDate(date + std::chrono::weeks{1}));
// EXPECT_TRUE(occurrence1.isForDate(date + std::chrono::weeks{2}));
// EXPECT_TRUE(occurrence1.isForDate(date + std::chrono::days{7}));
//
// auto occurrence2{Occurrence{date, std::chrono::months{1}}};
// EXPECT_TRUE(occurrence2.isForDate(date + std::chrono::months{1}));
// EXPECT_TRUE(occurrence2.isForDate(date + std::chrono::months{2}));
// EXPECT_TRUE(occurrence2.isForDate(date + std::chrono::months{12}));

// auto occurrence3{Occurrence{date, std::chrono::years{1}}};
// EXPECT_TRUE(occurrence3.isForDate(date + std::chrono::years{1}));
// EXPECT_TRUE(occurrence3.isForDate(date + std::chrono::years{2}));
// EXPECT_TRUE(occurrence3.isForDate(date + std::chrono::months{12}));
// }

// TEST(OccurrenceTest, IsForDateTrueForSelf) {
//     using hbt::mods::Occurrence, hbt::mods::Date;
//
//     auto date1{Date{
//         std::chrono::year{2026},
//         std::chrono::month{3},
//         std::chrono::day{16},
//     }};
//
//     auto occurrence{Occurrence{date1, 7}};
//
//     EXPECT_TRUE(occurrence.isForDate(date1));
// }

// TEST(OccurrenceTest, IsForDateFalseForPastDate) {
//     using hbt::mods::Occurrence, hbt::mods::Date;
//
//     auto date1{Date{
//         std::chrono::year{2026},
//         std::chrono::month{3},
//         std::chrono::day{23},
//     }};
//
//     auto date2{Date{
//         std::chrono::year{2026},
//         std::chrono::month{3},
//         std::chrono::day{16},
//     }};
//
//     auto occurrence{Occurrence{date1, 7}};
//
//     EXPECT_FALSE(occurrence.isForDate(date2));
// }

// TEST(OccurrenceTest, IsForDateFalseForWrongDate) {
//     using hbt::mods::Occurrence, hbt::mods::Date;
//
//     auto date1{Date{
//         std::chrono::year{2026},
//         std::chrono::month{3},
//         std::chrono::day{16},
//     }};
//
//     auto date2{Date{
//         std::chrono::year{2026},
//         std::chrono::month{3},
//         std::chrono::day{17},
//     }};
//
//     auto occurrence{Occurrence{date1, 7}};
//
//     EXPECT_FALSE(occurrence.isForDate(date2));
// }
} // namespace test::mods
