#pragma once

#include <nlohmann/json.hpp>

#include <duration_units.hpp>

#include <optional>

namespace hbt::mods {
class Interval {
  public:
    using value_t = hbt::mods::util::DurationUnits::value_t;

  private:
    hbt::mods::util::DurationUnits durationUnits_;

  public:
    Interval();

  private:
    Interval(hbt::mods::util::DurationUnits durationUnits);

  public:
    [[nodiscard]] static auto years(value_t value) -> Interval;

    [[nodiscard]] static auto months(value_t value) -> Interval;

    [[nodiscard]] static auto weeks(value_t value) -> Interval;

    [[nodiscard]] static auto days(value_t value) -> Interval;

    [[nodiscard]] static auto hours(value_t value) -> Interval;

    [[nodiscard]] static auto minutes(value_t value) -> Interval;

  public:
    [[nodiscard]] auto operator+(const Interval &other) const -> Interval;

  public:
    [[nodiscard]] auto isZero() const -> bool;

  public:
    [[nodiscard]] auto toISO8601String() const -> std::string;

    [[nodiscard]] static auto fromISO8601String(const std::string &string)
        -> std::optional<Interval>;
};
} // namespace hbt::mods
