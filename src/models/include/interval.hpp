#pragma once

#include <nlohmann/json.hpp>

#include <duration_units.hpp>

#include <optional>

namespace hbt::mods {
class Interval {
  public:
    enum class MonthHandling : char { CUT_OFF, WRAP_AROUND };

  private:
    Interval(hbt::mods::DurationUnits durationUnits,
             MonthHandling monthHandling = MonthHandling::CUT_OFF);

  private:
    hbt::mods::DurationUnits durationUnits_;
    MonthHandling monthHandling_;

  public:
    using value_t = hbt::mods::DurationUnits::value_t;

  public:
    Interval(MonthHandling monthHandling = MonthHandling::CUT_OFF);

  public:
    [[nodiscard]] static auto years(value_t value) -> Interval;

    [[nodiscard]] static auto months(value_t value) -> Interval;

    [[nodiscard]] static auto weeks(value_t value) -> Interval;

    [[nodiscard]] static auto days(value_t value) -> Interval;

    [[nodiscard]] static auto hours(value_t value) -> Interval;

    [[nodiscard]] static auto minutes(value_t value) -> Interval;

  public:
    [[nodiscard]] auto getYears() const -> value_t;

    [[nodiscard]] auto getMonths() const -> value_t;

    [[nodiscard]] auto getWeeks() const -> value_t;

    [[nodiscard]] auto getDays() const -> value_t;

    [[nodiscard]] auto getHours() const -> value_t;

    [[nodiscard]] auto getMinutes() const -> value_t;

  public:
    [[nodiscard]] auto getMonthHandling() const -> MonthHandling;

  public:
    auto setMonthHandling(MonthHandling monthHandling) -> void;

  public:
    [[nodiscard]] auto operator+(const Interval &other) const -> Interval;

    [[nodiscard]] auto operator<=>(const Interval &other) const
        -> std::strong_ordering = default;

    [[nodiscard]] auto operator==(const Interval &other) const
        -> bool = default;

  public:
    [[nodiscard]] auto isZero() const -> bool;

  public:
    [[nodiscard]] auto toISO8601String() const -> std::string;

    [[nodiscard]] static auto fromISO8601String(const std::string &string)
        -> std::optional<Interval>;
};
} // namespace hbt::mods
