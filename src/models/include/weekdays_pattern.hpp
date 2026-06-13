#pragma once

#include <calendar.hpp>
#include <recurrence_pattern.hpp>

namespace hbt::mods::util {
class WeekdaysRecurrencePattern : public RecurrencePattern {
  public:
    enum class Error : uint8_t {
        InvalidInterval,
        EmptyWeek,
    };

  public:
    [[nodiscard]] static constexpr auto errorMessage(Error error)
        -> std::string {
        switch (error) {
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
    /* don't change the order */
    clndr::Week firstCalendarWeek_;
    Interval interval_;
    Week week_;

  private:
    static auto validateInterval(const Interval &interval) -> Interval;

    static auto validateWeek(const Week &week) -> Week;

  private:
    [[nodiscard]] static auto getFirstOccurrence(DateTime start, Week week)
        -> DateTime;

  public:
    WeekdaysRecurrencePattern(DateTime start, Week week, Interval interval);

    WeekdaysRecurrencePattern(clndr::Week firstCalendarWeek, Week week,
                              Interval interval);

  public:
    [[nodiscard]] auto getInterval() const -> Interval;

    [[nodiscard]] auto getFirstCalendarWeek() const -> clndr::Week;

    [[nodiscard]] auto getWeek() const -> Week;

  public:
    [[nodiscard]] auto happensOnDate(DateTime on) const -> bool;

  public:
    [[nodiscard]] auto getOccurrencesOfDate(DateTime on) const
        -> occurrences_t override;

  public:
    struct JSON {
        enum class Error : uint8_t {
            MissingRequiredField,

            FailedToParseFirstCalendarWeek,
            FailedToParseInterval,
            FailedToParseWeek,

            FailedToValidateInterval,
            FailedToValidateWeek,
        };

        [[nodiscard]] static constexpr auto errorMessage(JSON::Error error)
            -> std::string {
            switch (error) {
            case JSON::Error::MissingRequiredField:
                return "WeekdayRecurrencePattern::JSON: missing required "
                       "field(s)";

            case JSON::Error::FailedToParseInterval:
                return "WeekdayRecurrencePattern::JSON: failed to parse "
                       "Interval";

            case JSON::Error::FailedToParseFirstCalendarWeek:
                return "WeekdayRecurrencePattern::JSON: failed to parse "
                       "clndr::Week";

            case JSON::Error::FailedToParseWeek:
                return "WeekdayRecurrencePattern::JSON: failed to parse Week";

            case JSON::Error::FailedToValidateInterval:
                return "WeekdayRecurrencePattern::JSON failed to validate "
                       "parsed Interval";

            case JSON::Error::FailedToValidateWeek:
                return "WeekdayRecurrencePattern::JSON failed to validate "
                       "parsed Week";
            }
        }

        static constexpr auto firstCalendarWeekField{
            std::string_view{"first_calendar_week"}};
        static constexpr auto intervalField{std::string_view{"interval"}};
        static constexpr auto weekField{std::string_view{"week"}};

        static constexpr auto fields{std::array<std::string_view, 3>{
            firstCalendarWeekField, intervalField, weekField}};

        [[nodiscard]] auto static encode(
            const WeekdaysRecurrencePattern &pattern) -> nlohmann::json;

        [[nodiscard]] auto static decode(const nlohmann::json &json)
            -> std::expected<WeekdaysRecurrencePattern, JSON::Error>;

        [[nodiscard]] auto static containsAllFields(const nlohmann::json &json)
            -> bool;
    };
};
} // namespace hbt::mods::util
