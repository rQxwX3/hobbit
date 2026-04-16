#pragma once

#include <nlohmann/json.hpp>

#include <date.hpp>
#include <interval.hpp>

#include <optional>
#include <variant>

namespace hbt::mods {

class Occurrence {
  private:
    hbt::mods::Date date_;

    struct NonRecurrent {
        auto operator==(const NonRecurrent &) const -> bool = default;

        [[nodiscard]] static auto toJSON() -> nlohmann::json {
            return {{"type", "non-recurrent"}};
        }

        [[nodiscard]] static auto fromJSON(const nlohmann::json &json)
            -> std::optional<NonRecurrent> {
            return NonRecurrent{};
        }
    };

    struct IntervalRecurrent {
        hbt::mods::Interval interval;

        auto operator==(const IntervalRecurrent &other) const -> bool = default;

        [[nodiscard]] auto toJSON() const -> nlohmann::json {
            return {{"type", "interval-recurrent"},
                    {"interval", interval.toJSON()}};
        }

        [[nodiscard]] static auto fromJSON(const nlohmann::json &json)
            -> std::optional<IntervalRecurrent> {
            if (!json.contains("interval")) {
                return std::nullopt;
            }

            auto intervalFromJSON{
                hbt::mods::Interval::fromJSON(json["interval"])};
            if (!intervalFromJSON.has_value()) {
                return std::nullopt;
            }

            return IntervalRecurrent{intervalFromJSON.value()};
        }
    };

    struct WeekdayRecurrent {
        hbt::mods::Date::weekdays_t weekdays;
        hbt::mods::Interval interval;

        auto operator==(const WeekdayRecurrent &other) const -> bool = default;

        [[nodiscard]] auto toJSON() const -> nlohmann::json {
            return {
                {"type", "weekday-recurrent"},
                {"weekdays", weekdays.to_string()},
                {"interval", interval.toJSON()},
            };
        }

        [[nodiscard]] static auto fromJSON(const nlohmann::json &json)
            -> std::optional<WeekdayRecurrent> {
            if (!json.contains("interval") || !json.contains("weekdays")) {
                return std::nullopt;
            }

            auto intervalFromJSON{
                hbt::mods::Interval::fromJSON(json["interval"])};

            if (!intervalFromJSON.has_value()) {
                return std::nullopt;
            }

            return WeekdayRecurrent{
                .weekdays =
                    Date::weekdays_t{json["weekdays"].get<std::string>()},
                .interval = intervalFromJSON.value()};
        }
    };

    std::variant<NonRecurrent, IntervalRecurrent, WeekdayRecurrent>
        recurrenceModel_;

  private:
    using recurrenceModel_t =
        std::variant<NonRecurrent, IntervalRecurrent, WeekdayRecurrent>;

    [[nodiscard]] static auto
    recurrenceModelFromJSON(const nlohmann::json &json)
        -> std::optional<recurrenceModel_t>;

  public:
    Occurrence(hbt::mods::Date date = hbt::mods::Date{});

    Occurrence(hbt::mods::Date date, const hbt::mods::Interval &interval);

    Occurrence(hbt::mods::Date date,
               std::vector<hbt::mods::Date::weekday_t> weekdays,
               const hbt::mods::Interval &interval);

    Occurrence(hbt::mods::Date date, recurrenceModel_t recurrenceModel);

  public:
    [[nodiscard]] auto getDate() const -> hbt::mods::Date;

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
