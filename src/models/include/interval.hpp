#pragma once

#include <nlohmann/json.hpp>

#include <duration.hpp>

#include <optional>

namespace hbt::mods {
class Interval {
  public:
    enum class MonthHandling : char { CUT_OFF, WRAP_AROUND };

  public:
    constexpr static auto defaultMonthHandling{MonthHandling::WRAP_AROUND};

  public:
    using value_t = hbt::mods::Duration::value_t;
    using unit_t = hbt::mods::Duration::unit_t;

  private:
  private:
    hbt::mods::Duration duration_;
    MonthHandling monthHandling_;

  public:
    Interval(hbt::mods::Duration duration,
             MonthHandling monthHandling = defaultMonthHandling);

    Interval(const Interval &other);

  public:
    explicit Interval(
        hbt::mods::Duration::Units units = hbt::mods::Duration::Units{},
        MonthHandling monthHandling = defaultMonthHandling);

  public:
    [[nodiscard]] static auto years(value_t value) -> Interval;

    [[nodiscard]] static auto months(value_t value) -> Interval;

    [[nodiscard]] static auto weeks(value_t value) -> Interval;

    [[nodiscard]] static auto days(value_t value) -> Interval;

    [[nodiscard]] static auto hours(value_t value) -> Interval;

    [[nodiscard]] static auto minutes(value_t value) -> Interval;

  public:
    [[nodiscard]] auto getUnitValue(unit_t unit) const -> value_t;

  public:
    [[nodiscard]] auto getMonthHandling() const -> MonthHandling;

    [[nodiscard]] auto getDuration() const -> Duration;

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

    [[nodiscard]] auto onlyContainsUnit(unit_t unit) const -> bool;

  public:
    [[nodiscard]] auto toJSON() const -> nlohmann::json;

    [[nodiscard]] static auto fromJSON(const nlohmann::json &json)
        -> std::optional<Interval>;

  public:
    [[nodiscard]] static auto fromNaturalLanguage(const std::string &input)
        -> std::optional<Interval>;

    [[nodiscard]] auto toNaturalLanguage() const -> std::string;
};
} // namespace hbt::mods
