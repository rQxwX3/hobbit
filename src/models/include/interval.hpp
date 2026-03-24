#pragma once

#include <nlohmann/json.hpp>

#include <duration_units.hpp>

#include <optional>

namespace hbt::mods {
class Interval {
  public:
    enum class MonthHandling : char { CUT_OFF, WRAP_AROUND };

  public:
    constexpr static auto defaultMonthHandling{MonthHandling::WRAP_AROUND};

  public:
    using value_t = hbt::mods::DurationUnits::value_t;

  private:
    hbt::mods::DurationUnits durationUnits_;
    MonthHandling monthHandling_;

  public:
    explicit Interval(hbt::mods::DurationUnits durationUnits,
                      MonthHandling monthHandling = defaultMonthHandling);

    explicit Interval(value_t years = 0, value_t months = 0, value_t weeks = 0,
                      value_t days = 0, value_t hours = 0, value_t minutes = 0,
                      MonthHandling monthHandling = defaultMonthHandling);

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
    [[nodiscard]] auto toJSON() const -> nlohmann::json;

    [[nodiscard]] static auto fromJSON(const nlohmann::json &json)
        -> std::optional<Interval>;
};
} // namespace hbt::mods
