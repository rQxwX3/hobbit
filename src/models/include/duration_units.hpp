#pragma once

#include <array>
#include <cstdint>
#include <optional>
#include <string>

namespace hbt::mods {
class DurationUnits {
  public:
    using unit_t = enum : uint8_t {
        YEAR,
        MONTH,
        WEEK,
        DAY,
        HOUR,
        MINUTE,
        COUNT_,
    };

    using value_t = std::size_t;
    using array_t = std::array<value_t, unit_t::COUNT_>;

    struct Units {
        value_t years{0};
        value_t months{0};
        value_t weeks{0};
        value_t days{0};
        value_t hours{0};
        value_t minutes{0};

        [[nodiscard]] auto toArray() const -> array_t {
            return array_t{years, months, weeks, days, hours, minutes};
        }
    };

  public:
    static constexpr value_t monthsInYear{12};
    static constexpr value_t weeksInMonth{4};
    static constexpr value_t daysInWeek{7};
    static constexpr value_t hoursInDay{24};
    static constexpr value_t minutesInHour{60};

    static constexpr value_t maxValue{999};

  private:
    array_t units_;

  public:
    DurationUnits();

    explicit DurationUnits(array_t unitsArray);

    explicit DurationUnits(const Units &unitsStruct);

  public:
    auto addYears(value_t value) -> void;

    auto addMonths(value_t value) -> void;

    auto addWeeks(value_t value) -> void;

    auto addDays(value_t value) -> void;

    auto addHours(value_t value) -> void;

    auto addMinutes(value_t value) -> void;

  public:
    [[nodiscard]] auto getUnit(unit_t unit) const -> value_t;

  public:
    [[nodiscard]] auto isZero() const -> bool;

  public:
    [[nodiscard]] auto operator+(const DurationUnits &other) const
        -> DurationUnits;

    [[nodiscard]] auto operator<=>(const DurationUnits &other) const
        -> std::strong_ordering;

    [[nodiscard]] auto operator==(const DurationUnits &other) const
        -> bool = default;

  public:
    [[nodiscard]] auto toISO8601String() const -> std::string;

    [[nodiscard]] static auto fromISO8601String(const std::string &string)
        -> std::optional<DurationUnits>;

    [[nodiscard]] static auto fromNaturalLanguage(const std::string &input)
        -> std::optional<DurationUnits>;
};
} // namespace hbt::mods
