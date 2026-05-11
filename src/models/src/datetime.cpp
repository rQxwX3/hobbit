#include <datetime.hpp>

#include <format>
#include <regex>

namespace hbt::mods {
using std::chrono::system_clock;

DateTime::DateTime() : value_{DateTime::now().value_} {}

DateTime::DateTime(value_t value) : value_{value} {}

DateTime::DateTime(Date date, Time time)
    : value_{duration_t(date.toDuration() + time.toDuration())} {
    if (!date.ok()) {
        throw std::invalid_argument(errorMessage(Error::InvalidDate));
    }

    if (!time.ok()) {
        throw std::invalid_argument(errorMessage(Error::InvalidTime));
    }
}

[[nodiscard]] auto DateTime::getDaysSinceEpoch() const -> Date::duration_t {
    return std::chrono::duration_cast<Date::duration_t>(
        value_.time_since_epoch());
}

[[nodiscard]] auto DateTime::getMinutesSinceMidnight() const
    -> Time::duration_t {
    using namespace std::chrono;

    auto days{floor<std::chrono::days>(value_)};
    auto timeOfDay{value_ - days};

    return duration_cast<Time::duration_t>(timeOfDay);
}

[[nodiscard]] auto DateTime::now() -> DateTime {
    using namespace std::chrono;

    return floor<duration_t>(system_clock::now());
}

[[nodiscard]] auto DateTime::getDate() const -> Date {
    using namespace std::chrono;

    auto ymd{year_month_day(floor<Date::duration_t>(value_))};

    return {
        .year = static_cast<Date::year_t>(int(ymd.year())),
        .month = static_cast<Date::month_t>(unsigned(ymd.month())),
        .day = static_cast<Date::day_t>(unsigned(ymd.day())),
    };
}

[[nodiscard]] auto DateTime::getTime() const -> Time {
    auto tod{value_ - std::chrono::floor<std::chrono::days>(value_)};
    auto hms{std::chrono::hh_mm_ss{tod}};

    return {.hour = static_cast<Time::hour_t>(hms.hours().count()),
            .minute = static_cast<Time::minute_t>(hms.minutes().count())};
}

[[nodiscard]] auto DateTime::getWeekday() const -> weekday_t {
    auto days{std::chrono::floor<Date::duration_t>(value_)};
    auto wd{std::chrono::weekday(days)};

    return static_cast<weekday_t>(wd.c_encoding());
}

[[nodiscard]] auto DateTime::equalDate(DateTime dt1, DateTime dt2) -> bool {
    return dt1.getDaysSinceEpoch() == dt2.getDaysSinceEpoch();
}

[[nodiscard]] auto DateTime::equalTime(DateTime dt1, DateTime dt2) -> bool {
    return dt1.getMinutesSinceMidnight() == dt2.getMinutesSinceMidnight();
}

[[nodiscard]] auto DateTime::toISO8601String() const -> std::string {
    return std::format("{:%Y-%m-%dT%H:%M}", value_);
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

    auto date{Date(yearValue, monthValue, dayValue)};
    if (!date.ok()) {
        return std::unexpected(Error::ISO8601InvalidDate);
    }

    auto time{Time(hourValue, minuteValue)};
    if (!time.ok()) {
        return std::unexpected(Error::ISO8601InvalidTime);
    }

    return DateTime(date, time);
}

[[nodiscard]] auto DateTime::operator<=>(const DateTime &other) const
    -> std::strong_ordering = default;

[[nodiscard]] auto DateTime::operator==(const DateTime &other) const
    -> bool = default;

[[nodiscard]] auto DateTime::operator+(const Interval &interval) const
    -> DateTime {
    using namespace std::chrono;
    using Unit = Interval::Unit;

    auto addYearsAndMonths(
        [](year_month_day &ymd, const Interval &interval) -> void {
            using namespace std::chrono;

            auto intervalYears{interval[Interval::Unit::YEAR]};
            auto intervalMonths{interval[Interval::Unit::MONTH]};

            switch (interval.getMonthHandling()) {
            case Interval::MonthHandling::WrapAround:
                ymd += years(intervalYears);
                ymd += months(intervalMonths);

                break;

            case Interval::MonthHandling::PreserveRelative:
                auto ym{year_month(ymd.year() / ymd.month())};
                auto oldDay(ymd.day());
                auto daysUntilOldMonthEnd(year_month_day_last(ym / last).day() -
                                          oldDay);

                ym += years(intervalYears);
                ym += months(intervalMonths);
                auto newMonthEnd{year_month_day_last(ym / last).day()};

                const auto largestCommonDayOfTheMonth{size_t{27}};
                const auto oldDayIsNotContainedByAllMonths{
                    static_cast<size_t>(unsigned(oldDay)) >
                    largestCommonDayOfTheMonth};

                auto newDay{(oldDayIsNotContainedByAllMonths)
                                ? newMonthEnd - daysUntilOldMonthEnd
                                : oldDay};

                ymd = year_month_day{ym / newDay};
            }
        });

    auto addWeeksAndDays([](year_month_day &ymd, Interval::value_t weeks,
                            Interval::value_t days) -> void {
        auto sysdays{sys_days(ymd)};
        sysdays += std::chrono::days((weeks * Interval::daysInWeek) + days);

        ymd = year_month_day(sysdays);
    });

    auto addHoursAndMinutes([](auto &timepoint, Interval::value_t hours,
                               Interval::value_t minutes) -> void {
        timepoint += std::chrono::hours(hours);
        timepoint += std::chrono::minutes(minutes);
    });

    auto dateInDays{floor<days>(value_)};
    auto timeOfDay{value_ - dateInDays};
    auto ymd{year_month_day(dateInDays)};

    addYearsAndMonths(ymd, interval);
    addWeeksAndDays(ymd, interval[Unit::WEEK], interval[Unit::DAY]);

    auto timepoint{sys_days(ymd) + timeOfDay};
    addHoursAndMinutes(timepoint, interval[Unit::HOUR], interval[Unit::MINUTE]);

    return {timepoint};
}

auto DateTime::operator+=(const Interval &interval) -> DateTime & {
    *this = *this + interval;
    return *this;
}

[[nodiscard]] auto DateTime::diff(const DateTime &dt1, const DateTime &dt2)
    -> Interval {
    using namespace std::chrono;

    auto diff{(dt1 > dt2) ? (dt1.value_ - dt2.value_)
                          : (dt2.value_ - dt1.value_)};

    return Interval::minutes(diff.count());
}

[[nodiscard]] auto DateTime::daysDiff(const DateTime &dt1, const DateTime &dt2)
    -> Interval {
    auto minutesDiff{DateTime::diff(dt1, dt2)[Interval::Unit::MINUTE]};

    return Interval::days(minutesDiff / Interval::minutesInDay);
}
}; // namespace hbt::mods
