#pragma once

#include <array>
#include <cstdint>
#include <expected>
#include <optional>
#include <string>
#include <utility>
#include <vector>

namespace hbt::mods {
class Interval {
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
            return "Interval: failed to parse from JSON";

        case Error::NaturalLanguageFailedToParse:
            return "Interval: failed to parse from natural language";

        case Error::InvalidValue:
            return "Interval: provided value is too high (possible sign "
                   "overflow)";

        case Error::InvalidArray:
            return "Interval: provided array contains invalid value(s)";

        case Error::InvalidStruct:
            return "Interval: provided struct contains invalid value(s)";

        default:
            return "Interval: unclassified error";
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

        [[nodiscard]] static auto fromArray(const array_t &array) -> Units {
            return {.years = array[unit_t::YEAR],
                    .months = array[unit_t::MONTH],
                    .weeks = array[unit_t::WEEK],
                    .days = array[unit_t::DAY],
                    .hours = array[unit_t::HOUR],
                    .minutes = array[unit_t::MINUTE]};
        }
    };

    using struct_t = Units;

  public:
    static constexpr auto maxValue{value_t{999}};

  public:
    static constexpr auto minutesInHour{value_t{60}};
    static constexpr auto hoursInDay{value_t{24}};

  public:
    static constexpr auto units{std::array<unit_t, unit_t::COUNT_>{
        unit_t::YEAR, unit_t::MONTH, unit_t::WEEK, unit_t::DAY, unit_t::HOUR,
        unit_t::MINUTE}};

  private:
    array_t units_;

  private:
    [[nodiscard]] auto getMaxNonZeroUnit() const -> std::optional<unit_t>;

  private:
    static auto validateValue(value_t value) -> value_t;

    static auto validateArray(array_t array) -> array_t;

    [[nodiscard]] static auto validateStruct(struct_t unitsStruct) -> struct_t;

  public:
    Interval();

    explicit Interval(array_t unitsArray);

    explicit Interval(const struct_t &unitsStruct);

  public:
    [[nodiscard]] auto convertUnitsUpwards() const -> Interval;

    [[nodiscard]] auto convertUnitsDownwards() const -> Interval;

  public:
    [[nodiscard]] static auto fromUnit(unit_t unit, value_t value) -> Interval;

  public:
    [[nodiscard]] static auto years(value_t value) -> Interval;

    [[nodiscard]] static auto months(value_t value) -> Interval;

    [[nodiscard]] static auto weeks(value_t value) -> Interval;

    [[nodiscard]] static auto days(value_t value) -> Interval;

    [[nodiscard]] static auto hours(value_t value) -> Interval;

    [[nodiscard]] static auto minutes(value_t value) -> Interval;

  public:
    auto addUnit(unit_t unit, value_t value) -> void;

  public:
    [[nodiscard]] auto getUnitValue(unit_t unit) const -> value_t;

    [[nodiscard]] auto getUnits() const -> Units;

    [[nodiscard]] auto getNonZeroUnitValuePairs() const
        -> std::vector<unitValuePair_t>;

  public:
    [[nodiscard]] auto isZero() const -> bool;

    [[nodiscard]] auto onlyContainsUnit(unit_t onlyUnit) const -> bool;

    [[nodiscard]] auto isMultipleOf(Interval other) const -> bool;

    [[nodiscard]] static auto isValidValue(value_t value) -> bool;

  public:
    [[nodiscard]] auto operator+(const Interval &other) const -> Interval;

    [[nodiscard]] auto operator<=>(const Interval &other) const
        -> std::strong_ordering;

    [[nodiscard]] auto operator==(const Interval &other) const -> bool;

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
