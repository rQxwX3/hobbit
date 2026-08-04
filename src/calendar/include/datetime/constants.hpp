#pragma once

#include <array>
#include <cstdint>

namespace clndr::dt::constants {
enum class Weekday : uint8_t {
    SUNDAY,
    MONDAY,
    TUESDAY,
    WEDNESDAY,
    THURSDAY,
    FRIDAY,
    SATURDAY,
    COUNT_,
};

static constexpr auto weekdaysCount{static_cast<size_t>(Weekday::COUNT_)};
static constexpr auto weekdaysArray{std::array<Weekday, weekdaysCount>{
    {Weekday::SUNDAY, Weekday::MONDAY, Weekday::TUESDAY, Weekday::WEDNESDAY,
     Weekday::THURSDAY, Weekday::FRIDAY, Weekday::SATURDAY}}};

static constexpr auto daysInNonLeapFebruary{size_t{28}};
static constexpr auto daysInLeapFebruary{daysInNonLeapFebruary + 1};

static constexpr auto daysInWeek{size_t{7}};
static constexpr auto monthsInYear{size_t{12}};
static constexpr auto minutesInHour{size_t{60}};
static constexpr auto hoursInDay{size_t{24}};
static constexpr auto minutesInDay{minutesInHour * hoursInDay};

static constexpr auto minHourValue{0};
static constexpr auto maxHourValue{23};

static constexpr auto minMinuteValue{0};
static constexpr auto maxMinuteValue{59};
} // namespace clndr::dt::constants
