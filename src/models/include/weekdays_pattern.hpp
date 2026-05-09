#pragma once

#include <recurrence_pattern.hpp>

namespace hbt::mods::util {
class WeekdaysRecurrencePattern : public RecurrencePattern {
  private:
    enum class Error : uint8_t {
        JSONMissingRequiredField,

        JSONFailedToParseStart,
        JSONFailedToParseInterval,
        JSONFailedToParseWeekdays,

        InvalidInterval,
        InvalidWeekdays,

        EmptyWeekdays,
    };

  public:
    [[nodiscard]] static constexpr auto errorMessage(Error error)
        -> std::string {
        switch (error) {
        case Error::JSONMissingRequiredField:
            return "WeekdayRecurrencePattern: missing required field(s) in "
                   "JSON";

        case Error::JSONFailedToParseStart:
            return "WeekdayRecurrencePattern: failed to parse start DateTime "
                   "from JSON";

        case Error::JSONFailedToParseInterval:
            return "WeekdayRecurrencePattern: failed to parse Interval from "
                   "JSON";

        case Error::JSONFailedToParseWeekdays:
            return "WeekdayRecurrencePattern: failed to parse Weekdays from "
                   "JSON";

        case Error::InvalidInterval:
            return "WeekdayRecurrencePattern: provided Interval contains units "
                   "other than week";

        case Error::InvalidWeekdays:
            return "WeekdayRecurrencePattern: provided Weekdays do not contain "
                   "single selected day";

        case Error::EmptyWeekdays:
            return "WeekdayRecurrencePattern: invalid object state (empty "
                   "Weekdays object)";

        default:
            std::unreachable();
        }
    }

  private:
    static constexpr auto jsonStartField{std::string_view{"start"}};
    static constexpr auto jsonIntervalField{std::string_view{"interval"}};
    static constexpr auto jsonWeekdaysField{std::string_view{"weekdays"}};

    static constexpr auto jsonFields{std::array<std::string_view, 3>{
        jsonStartField, jsonIntervalField, jsonWeekdaysField}};

  private:
    DateTime start_;
    Interval interval_;
    Weekdays weekdays_;

  private:
    static auto validateInterval(const Interval &interval) -> Interval;

    static auto validateWeekdays(Weekdays weekdays) -> Weekdays;

  public:
    WeekdaysRecurrencePattern(DateTime start, Interval interval,
                              Weekdays weekdays);

  public:
    [[nodiscard]] auto getInterval() const -> Interval;

    [[nodiscard]] auto getWeekdays() const -> Weekdays;

  public:
    [[nodiscard]] auto happensOnDate(DateTime on) const -> bool;

  public:
    [[nodiscard]] auto getOccurrencesOfDate(DateTime on) const
        -> occurrences_t override;

  private:
    [[nodiscard]] auto static containsAllJSONFields(const nlohmann::json &json)
        -> bool;

  public:
    [[nodiscard]] auto toJSON() const -> nlohmann::json;

    [[nodiscard]] auto static fromJSON(const nlohmann::json &json)
        -> std::expected<WeekdaysRecurrencePattern, Error>;
};
} // namespace hbt::mods::util
