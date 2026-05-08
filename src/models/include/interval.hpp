#pragma once

#include <array>
#include <cstdint>
#include <expected>
#include <limits>
#include <string>
#include <utility>
#include <vector>

namespace hbt::mods {
class Interval {
  public:
    enum class Error : uint8_t {
        ISO8601FailedToParse,
        NaturalLanguageFailedToParse,

        InvalidComparison,
    };

    [[nodiscard]] static constexpr auto errorMessage(Error error)
        -> std::string {
        switch (error) {
        case Error::ISO8601FailedToParse:
            return "Interval: failed to parse from JSON";

        case Error::NaturalLanguageFailedToParse:
            return "Interval: failed to parse from natural language";

        case Error::InvalidComparison:
            return "Interval: can't compare month-based and minute-based "
                   "intervals";

        default:
            std::unreachable();
        }
    }

  public:
    using Unit = enum : uint8_t {
        YEAR,
        MONTH,
        WEEK,
        DAY,
        HOUR,
        MINUTE,
        COUNT_,
    };

    using value_t = std::uint32_t;
    using unitValuePair_t = std::pair<Unit, value_t>;

    using array_t = std::array<value_t, Unit::COUNT_>;

    using struct_t = struct Units {
        value_t years{0};
        value_t months{0};
        value_t weeks{0};
        value_t days{0};
        value_t hours{0};
        value_t minutes{0};

        [[nodiscard]] auto toArray() const -> array_t {
            return array_t{years, months, weeks, days, hours, minutes};
        }

        [[nodiscard]] static auto fromArray(const array_t &array) -> Units {
            return {
                .years = array[Unit::YEAR],
                .months = array[Unit::MONTH],
                .weeks = array[Unit::WEEK],
                .days = array[Unit::DAY],
                .hours = array[Unit::HOUR],
                .minutes = array[Unit::MINUTE],
            };
        }
    };

  public:
    static constexpr value_t maxValue{std::numeric_limits<value_t>::max()};

    [[nodiscard]] static constexpr auto isValidValue(std::uint64_t value)
        -> bool {
        return value < maxValue;
    }

  public:
    enum class MonthHandling : uint8_t {
        WrapAround,
        PreserveRelative,
    };

    static constexpr auto defaultMonthHandling{MonthHandling::WrapAround};

  public:
    static constexpr auto daysInWeek{value_t{7}};
    static constexpr auto monthsInYear{value_t{12}};
    static constexpr auto minutesInHour{value_t{60}};
    static constexpr auto hoursInDay{value_t{24}};

    static constexpr auto minutesInDay{minutesInHour * hoursInDay};

  public:
    static constexpr auto units{
        std::array<Unit, Unit::COUNT_>{Unit::YEAR, Unit::MONTH, Unit::WEEK,
                                       Unit::DAY, Unit::HOUR, Unit::MINUTE}};

  private:
    array_t units_;
    MonthHandling monthHandling_;

  public:
    Interval(MonthHandling = defaultMonthHandling);

    explicit Interval(array_t array, MonthHandling = defaultMonthHandling);

    explicit Interval(const struct_t &units,
                      MonthHandling = defaultMonthHandling);

  public:
    [[nodiscard]] auto convertUnitsDownwards() const -> Interval;

  public:
    [[nodiscard]] static auto fromUnit(Unit unit, value_t value) -> Interval;

  public:
    [[nodiscard]] static auto years(value_t value) -> Interval;

    [[nodiscard]] static auto months(value_t value) -> Interval;

    [[nodiscard]] static auto weeks(value_t value) -> Interval;

    [[nodiscard]] static auto days(value_t value) -> Interval;

    [[nodiscard]] static auto hours(value_t value) -> Interval;

    [[nodiscard]] static auto minutes(value_t value) -> Interval;

  public:
    auto addUnit(Unit unit, value_t value) -> void;

  public:
    [[nodiscard]] auto getUnitsStruct() const -> struct_t;

    [[nodiscard]] auto getNonZeroUnitValuePairs() const
        -> std::vector<unitValuePair_t>;

    [[nodiscard]] auto getMonthHandling() const -> MonthHandling;

  public:
    [[nodiscard]] auto isZero() const -> bool;

    [[nodiscard]] auto onlyContainsUnit(Unit onlyUnit) const -> bool;

    [[nodiscard]] auto isMultipleOf(Interval other) const -> bool;

  public:
    [[nodiscard]] auto operator+(const Interval &other) const -> Interval;

    [[nodiscard]] auto operator<=>(const Interval &other) const
        -> std::strong_ordering;

    [[nodiscard]] auto operator==(const Interval &other) const -> bool;

    [[nodiscard]] auto operator[](Unit unit) const -> value_t;

  private:
    [[nodiscard]] static auto compare(const Interval &lhs, const Interval &rhs)
        -> bool;

  public:
    [[nodiscard]] auto toISO8601String() const -> std::string;

    [[nodiscard]] static auto fromISO8601String(const std::string &string)
        -> std::expected<Interval, Error>;

  public:
    [[nodiscard]] static auto fromNaturalLanguage(const std::string &input)
        -> std::expected<Interval, Error>;

    [[nodiscard]] auto toNaturalLanguage() const -> std::string;
};
} // namespace hbt::mods
