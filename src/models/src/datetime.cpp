#include <datetime.hpp>

#include <format>
#include <regex>

namespace hbt::mods {
using std::chrono::system_clock;

DateTime::DateTime()
    : date_{Date()},

      time_{[]() -> time_value_t {
          auto now{system_clock::now()};
          auto today{floor<std::chrono::days>(now)};
          return duration_cast<time_value_t>(now - today);
      }()} {}

DateTime::DateTime(mods::Date date, mods::Time time)
    : date_{date}, time_{time} {}

DateTime::DateTime(mods::Date date, time_value_t timeValue)
    : date_{date}, time_{mods::Time(timeValue)} {}

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
    -> std::expected<DateTime, Error> {
    const auto pattern{std::regex{
        R"(^(\d{4})[-./](\d{2})[-./](\d{2})[ T](\d{2}):(\d{2})(?::(\d{2}))?$)"}};

    std::smatch matches;
    if (!std::regex_match(string, matches, pattern)) {
        return std::unexpected(Error::ISO8601RegexMismatch);
    }

    constexpr size_t yearGroup{1};
    constexpr size_t monthGroup{2};
    constexpr size_t dayGroup{3};
    constexpr size_t hourGroup{4};
    constexpr size_t minuteGroup{5};

    constexpr size_t secondGroup{6}; // not supported, probably never will
                                     // be (currently discarding the value)

    constexpr auto groups{std::array<size_t, minuteGroup>{
        yearGroup, monthGroup, dayGroup, hourGroup, minuteGroup}};

    if (std::ranges::any_of(groups, [&matches](auto group) -> bool {
            return !matches[group].matched;
        })) {
        return std::unexpected(Error::ISO8601UnitNotMatched);
    }

    auto yearValue{std::stoi(matches[yearGroup].str())};
    auto monthValue{std::stoi(matches[monthGroup].str())};
    auto dayValue{std::stoi(matches[dayGroup].str())};
    auto hourValue{std::stoi(matches[hourGroup].str())};
    auto minuteValue{std::stoi(matches[minuteGroup].str())};

    try {
        auto date{Date(yearValue, monthValue, dayValue)};
        auto time{Time(hourValue, minuteValue)};

        return DateTime(date, time);
    } catch (std::invalid_argument) {
        return std::unexpected(Error::ISO8601InvalidDateTime);
    }
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

[[nodiscard]] auto DateTime::diff(const DateTime &dt1, const DateTime &dt2)
    -> Interval {
    auto dateDiff{Date::daysBetween(dt1.getDate(), dt2.getDate())};
    auto timeDiff{Time::minutesBetween(dt1.getTime(), dt2.getTime())};

    return dateDiff + timeDiff;
}
}; // namespace hbt::mods
