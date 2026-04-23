#include <datetime.hpp>

#include <format>
#include <optional>
#include <regex>

namespace hbt::mods {
using std::chrono::system_clock;

DateTime::DateTime()
    : date_{floor<std::chrono::days>(system_clock::now())},

      time_{[]() -> time_value_t {
          auto now{system_clock::now()};
          auto today{floor<std::chrono::days>(now)};
          return duration_cast<time_value_t>(now - today);
      }()} {}

DateTime::DateTime(mods::Date date, time_value_t time)
    : date_{date}, time_{time} {}

DateTime::DateTime(year_t year, month_t month, day_t day, hours_t hours,
                   minutes_t minutes)
    : date_{year, month, day}, time_{hours, minutes} {}

[[nodiscard]] auto DateTime::getDate() const -> mods::Date { return date_; }

[[nodiscard]] auto DateTime::now() -> DateTime {
    return DateTime(Date::today(), Time::now());
}

[[nodiscard]] auto DateTime::getTime() const -> mods::Time { return time_; }

[[nodiscard]] auto DateTime::equalDate(DateTime dt1, DateTime dt2) -> bool {
    return dt1.getDate() == dt2.getDate();
}

[[nodiscard]] auto DateTime::equalTime(DateTime dt1, DateTime dt2) -> bool {
    return dt1.getTime() == dt2.getTime();
}

[[nodiscard]] auto DateTime::toISO8601String() const -> std::string {
    auto timepoint{std::chrono::sys_days{date_.getYMD()} + time_.getValue()};
    return std::format("{:%Y-%m-%dT%H:%M}", timepoint);
}

[[nodiscard]] auto DateTime::fromISO8601String(const std::string &string)
    -> std::optional<DateTime> {
    const auto pattern{std::regex{
        R"(^(\d{4})[-./](\d{2})[-./](\d{2})[ T](\d{2}):(\d{2})(?::(\d{2}))?$)"}};

    constexpr size_t yearGroup{1};
    constexpr size_t monthGroup{2};
    constexpr size_t dayGroup{3};
    constexpr size_t hourGroup{4};
    constexpr size_t minuteGroup{5};
    constexpr size_t secondGroup{6}; // not supported, probably never will be

    std::smatch matches;
    if (!std::regex_match(string, matches, pattern)) {
        return std::nullopt;
    }

    if (!matches[yearGroup].matched || !matches[monthGroup].matched ||
        !matches[dayGroup].matched || !matches[hourGroup].matched ||
        !matches[minuteGroup].matched) {
        return std::nullopt;
    }

    const auto year{std::stoi(matches[yearGroup].str())};
    const auto month{
        static_cast<unsigned>(std::stoi(matches[monthGroup].str()))};
    const auto day{static_cast<unsigned>(std::stoi(matches[dayGroup].str()))};

    const auto date{mods::Date(year_t{year}, month_t{month}, day_t{day})};

    const auto hour{std::stoi(matches[hourGroup].str())};
    const auto minute{std::stoi(matches[minuteGroup].str())};

    if (hour < 0 || hour > DurationUnits::hoursInDay - 1 || minute < 0 ||
        minute > DurationUnits::minutesInHour - 1) {
        return std::nullopt;
    }

    auto time{duration_cast<time_value_t>(hours_t{hour}) + minutes_t{minute}};

    return DateTime{date, time};
}

[[nodiscard]] auto DateTime::operator<=>(const DateTime &other) const
    -> std::strong_ordering = default;

[[nodiscard]] auto DateTime::operator==(const DateTime &other) const
    -> bool = default;

[[nodiscard]] auto DateTime::operator+(const Interval &interval) const
    -> DateTime {
    auto [newTime, overflow]{getTime() + interval};
    auto newDate{getDate() +
                 ((overflow) ? interval + Interval::days(1) : interval)};

    return DateTime{newDate, newTime};
}

auto DateTime::operator+=(const Interval &interval) -> DateTime & {
    *this = *this + interval;
    return *this;
}

[[nodiscard]] auto DateTime::getDiff(const DateTime &dt1, const DateTime &dt2)
    -> DurationUnits {
    auto dateDiff{Date::getDiff(dt1.getDate(), dt2.getDate())};
    auto timeDiff{Time::getDiff(dt1.getTime(), dt2.getTime())};

    return dateDiff + timeDiff;
}
}; // namespace hbt::mods
