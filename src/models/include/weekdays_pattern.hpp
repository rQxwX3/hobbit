#pragma once

#include <recurrence_pattern.hpp>

namespace hbt::mods::util {
class WeekdaysRecurrencePattern : public RecurrencePattern {
  public:
    enum class Error : uint8_t {
        JSONMissingRequiredField,

        JSONFailedToParseFirstWeek,
        JSONFailedToParseInterval,

        JSONInvalidInterval,

        JSONFirstWeekNotArray,
        JSONFirstWeekInvalidCount,
        JSONFirstWeekArrayIsNotOfStrings,
        JSONFirstWeekFailedToParseDateTime,

        InvalidInterval,
        EmptyWeek,
    };

  public:
    [[nodiscard]] static constexpr auto errorMessage(Error error)
        -> std::string {
        switch (error) {
        case Error::JSONMissingRequiredField:
            return "WeekdayRecurrencePattern: missing required field(s) in "
                   "JSON";

        case Error::JSONFailedToParseInterval:
            return "WeekdayRecurrencePattern: failed to parse Interval from "
                   "JSON";

        case Error::InvalidInterval:
            return "WeekdayRecurrencePattern: provided Interval contains units "
                   "other than week";

        case Error::EmptyWeek:
            return "WeekdayRecurrencePattern: cannot construct pattern from "
                   "empty Week object";

        default:
            std::unreachable();
        }
    }

  private:
    static constexpr auto jsonFirstWeekField{
        std::string_view{"first_week_datetimes"}};
    static constexpr auto jsonIntervalField{std::string_view{"interval"}};

    static constexpr auto jsonFields{
        std::array<std::string_view, 2>{jsonFirstWeekField, jsonIntervalField}};

    static constexpr auto jsonFirstWeekNullValue{std::string_view{"null"}};

  public:
    using firstWeek_t =
        std::array<std::optional<DateTime>, Week::weekdaysCount>;

  private:
    [[nodiscard]] static auto createFirstWeek(DateTime startDT, Week week)
        -> firstWeek_t {
        auto firstOccurrence{DateTime()};
        for (auto dt{startDT};; dt += Interval::days(1)) {
            if (week.containsWeekday(dt.getWeekday())) {
                firstOccurrence = dt;
                break;
            }
        }

        auto firstWeek{firstWeek_t{}};
        auto endDT{firstOccurrence + Interval::weeks(1)};

        for (auto dt{firstOccurrence}; dt.getDate() != endDT.getDate();
             dt += Interval::days(1)) {
            auto dtWeekday{dt.getWeekday()};
            if (week.containsWeekday(dtWeekday)) {
                firstWeek[static_cast<size_t>(dtWeekday)] = dt;
            } else {
                firstWeek[static_cast<size_t>(dtWeekday)] = std::nullopt;
            }
        }

        return firstWeek;
    }

  private:
    firstWeek_t firstWeek_;
    Interval interval_;

  private:
    static auto validateInterval(const Interval &interval) -> Interval;

    static auto validateWeek(const Week &week) -> Week;

  public:
    WeekdaysRecurrencePattern(DateTime start, Week weekdays, Interval interval);

    WeekdaysRecurrencePattern(firstWeek_t firstWeek, Interval interval);

  public:
    [[nodiscard]] auto getInterval() const -> Interval;

    [[nodiscard]] auto getWeekdays() const -> Week;

  public:
    [[nodiscard]] auto happensOnDate(DateTime on) const -> bool;

  public:
    [[nodiscard]] auto getOccurrencesOfDate(DateTime on) const
        -> occurrences_t override;

  private:
    [[nodiscard]] auto static containsAllJSONFields(const nlohmann::json &json)
        -> bool;

  public:
    [[nodiscard]] auto firstWeekToJSON() const -> nlohmann::json;

    [[nodiscard]] auto static firstWeekFromJSON(const nlohmann::json &json)
        -> std::expected<firstWeek_t, Error>;

  public:
    [[nodiscard]] auto toJSON() const -> nlohmann::json;

    [[nodiscard]] auto static fromJSON(const nlohmann::json &json)
        -> std::expected<WeekdaysRecurrencePattern, Error>;
};
} // namespace hbt::mods::util
