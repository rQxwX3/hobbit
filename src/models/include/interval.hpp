#pragma once

#include <nlohmann/json.hpp>

#include <duration_units.hpp>

#include <optional>

namespace hbt::mods {
class Interval {
  private:
    enum class MonthHandling : char { NONE, CUT_OFF, WRAP_AROUND };

  private:
    Interval(hbt::mods::util::DurationUnits durationUnits,
             MonthHandling monthHandling = MonthHandling::NONE);

  private:
    hbt::mods::util::DurationUnits durationUnits_;
    MonthHandling monthHandling_;

  public:
    using value_t = hbt::mods::util::DurationUnits::value_t;

  public:
    Interval(MonthHandling monthHandling = MonthHandling::NONE);

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
    [[nodiscard]] auto operator+(const Interval &other) const -> Interval;

  public:
    [[nodiscard]] auto isZero() const -> bool;

  public:
    [[nodiscard]] auto toISO8601String() const -> std::string;

    [[nodiscard]] static auto fromISO8601String(const std::string &string)
        -> std::optional<Interval>;
};
} // namespace hbt::mods
