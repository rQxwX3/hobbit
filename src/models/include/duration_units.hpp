#pragma once

#include <array>
#include <cstdint>

namespace hbt::mods::util {
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

  private:
    array_t units_;

  public:
    DurationUnits();

    explicit DurationUnits(array_t units);

  public:
    auto addYears(value_t value) -> void;

    auto addMonths(value_t value) -> void;

    auto addWeeks(value_t value) -> void;

    auto addDays(value_t value) -> void;

    auto addHours(value_t value) -> void;

    auto addMinutes(value_t value) -> void;

  public:
    [[nodiscard]] auto getYears() const -> value_t;

    [[nodiscard]] auto getMonths() const -> value_t;

    [[nodiscard]] auto getWeeks() const -> value_t;

    [[nodiscard]] auto getDays() const -> value_t;

    [[nodiscard]] auto getHours() const -> value_t;

    [[nodiscard]] auto getMinutes() const -> value_t;
};
} // namespace hbt::mods::util
