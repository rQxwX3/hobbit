#pragma once

#include <nlohmann/json.hpp>

#include <date.hpp>
#include <interval.hpp>
#include <week.hpp>

#include <optional>
#include <variant>

namespace hbt::mods {
class Occurrence {
  public:
    struct NonRecurrent {
        auto operator==(const NonRecurrent &) const -> bool = default;

        [[nodiscard]] auto toJSON() const -> nlohmann::json;

        [[nodiscard]] static auto fromJSON(const nlohmann::json &json)
            -> std::optional<NonRecurrent>;
    };

    struct IntervalRecurrent {
        hbt::mods::Interval interval;

        auto operator==(const IntervalRecurrent &other) const -> bool = default;

        [[nodiscard]] auto toJSON() const -> nlohmann::json;

        [[nodiscard]] static auto fromJSON(const nlohmann::json &json)
            -> std::optional<IntervalRecurrent>;
    };

    struct WeekdayRecurrent {
        hbt::mods::Week week;
        hbt::mods::Interval interval;

        auto operator==(const WeekdayRecurrent &other) const -> bool = default;

        [[nodiscard]] auto toJSON() const -> nlohmann::json;

        [[nodiscard]] static auto fromJSON(const nlohmann::json &json)
            -> std::optional<WeekdayRecurrent>;
    };

  private:
    hbt::mods::Date date_;

    std::variant<NonRecurrent, IntervalRecurrent, WeekdayRecurrent>
        recurrenceModel_;

  public:
    using recurrencePattern_t =
        std::variant<NonRecurrent, IntervalRecurrent, WeekdayRecurrent>;

    [[nodiscard]] static auto
    recurrenceModelFromJSON(const nlohmann::json &json)
        -> std::optional<recurrencePattern_t>;

    [[nodiscard]] auto
    recurrenceModelToJSON(const recurrencePattern_t &recurrenceModel) const
        -> nlohmann::json;

  public:
    Occurrence(hbt::mods::Date date = hbt::mods::Date{},
               recurrencePattern_t recurrenceModel = NonRecurrent{});

    Occurrence(hbt::mods::Date date, const hbt::mods::Interval &interval);

    Occurrence(hbt::mods::Date date, hbt::mods::Week week,
               const hbt::mods::Interval &interval);

  public:
    [[nodiscard]] auto getDate() const -> hbt::mods::Date;

    [[nodiscard]] auto getRecurrenceModel() const -> recurrencePattern_t;

    [[nodiscard]] auto getWeekday() const -> hbt::mods::Date::weekday_t;

    [[nodiscard]] auto isRecurrent() const -> bool;

    [[nodiscard]] auto getInterval() const
        -> std::optional<hbt::mods::Interval>;

  public:
    [[nodiscard]] auto toJSON() const -> nlohmann::json;

    [[nodiscard]] static auto fromJSON(const nlohmann::json &json)
        -> std::optional<Occurrence>;

  public:
    [[nodiscard]] auto isForDate(Date date) const -> bool;

  public:
    [[nodiscard]] auto operator==(const Occurrence &other) const -> bool;
};
} // namespace hbt::mods
