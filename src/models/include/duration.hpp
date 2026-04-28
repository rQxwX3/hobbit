#pragma once

#include <array>
#include <cstdint>
#include <expected>
#include <optional>
#include <string>
#include <utility>
#include <vector>

namespace hbt::mods {
class Duration {
  private:
    enum class Error : uint8_t {
        ISO8601FailedToParse,
        NaturalLanguageFailedToParse,

        InvalidValue,
        InvalidArray,
        InvalidStruct,
    };

  public:
    [[nodiscard]] static constexpr auto errorMessage(Error error)
        -> std::string {
        switch (error) {
        case Error::ISO8601FailedToParse:
            return "Duration: failed to parse from JSON";

        case Error::NaturalLanguageFailedToParse:
            return "Duration: failed to parse from natural language";

        case Error::InvalidValue:
            return "Duration: provided value is too high (possible sign "
                   "overflow)";

        case Error::InvalidArray:
            return "Duration: provided array contains invalid value(s)";

        case Error::InvalidStruct:
            return "Duration: provided struct contains invalid value(s)";
        }
    }

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
    using unitValuePair_t = std::pair<unit_t, value_t>;

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

    using struct_t = Units;

  public:
    static constexpr auto maxValue{value_t{999}};

  public:
    static constexpr auto minutesInHour{value_t{60}};
    static constexpr auto hoursInDay{value_t{24}};
    static constexpr auto daysInWeek{value_t{7}};
    static constexpr auto monthsInYear{value_t{12}};

  private:
    array_t units_;

  private:
    [[nodiscard]] auto getMaxNonZeroUnit() const -> std::optional<unit_t>;

  private:
    static auto validateValue(value_t value) -> value_t;

    static auto validateArray(array_t array) -> array_t;

    [[nodiscard]] static auto validateStruct(struct_t unitsStruct) -> struct_t;

  public:
    Duration();

    explicit Duration(array_t unitsArray);

    explicit Duration(const struct_t &unitsStruct);

  private:
    [[nodiscard]] auto convertUnitsUpwards() const -> Duration;

    [[nodiscard]] auto convertUnitsDownwards() const -> Duration;

  public:
    [[nodiscard]] static auto fromUnit(unit_t unit, value_t value) -> Duration;

  public:
    [[nodiscard]] static auto years(value_t value) -> Duration;

    [[nodiscard]] static auto months(value_t value) -> Duration;

    [[nodiscard]] static auto weeks(value_t value) -> Duration;

    [[nodiscard]] static auto days(value_t value) -> Duration;

    [[nodiscard]] static auto hours(value_t value) -> Duration;

    [[nodiscard]] static auto minutes(value_t value) -> Duration;

  public:
    auto addUnit(unit_t unit, value_t value) -> void;

  public:
    [[nodiscard]] auto getUnitValue(unit_t unit) const -> value_t;

    [[nodiscard]] auto getNonZeroUnitValuePairs() const
        -> std::vector<unitValuePair_t>;

  public:
    [[nodiscard]] auto isZero() const -> bool;

    [[nodiscard]] auto onlyContainsUnit(unit_t onlyUnit) const -> bool;

    [[nodiscard]] auto isMultipleOf(Duration other) const -> bool;

    [[nodiscard]] static auto isValidValue(value_t value) -> bool;

  public:
    [[nodiscard]] auto operator+(const Duration &other) const -> Duration;

    [[nodiscard]] auto operator<=>(const Duration &other) const
        -> std::strong_ordering;

    [[nodiscard]] auto operator==(const Duration &other) const
        -> bool = default;

  public:
    [[nodiscard]] auto toISO8601String() const -> std::string;

    [[nodiscard]] static auto fromISO8601String(const std::string &string)
        -> std::expected<Duration, Error>;

  public:
    [[nodiscard]] static auto fromNaturalLanguage(const std::string &input)
        -> std::expected<Duration, Error>;

    [[nodiscard]] auto toNaturalLanguage() const -> std::string;
};
} // namespace hbt::mods
