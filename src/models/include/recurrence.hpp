#pragma once

#include <datetime.hpp>
#include <recurrence_patterns.hpp>

#include <nlohmann/json.hpp>

#include <array>
#include <expected>
#include <string_view>
#include <variant>

namespace hbt::mods::util {
class Recurrence {
  public:
    using pattern_t =
        std::variant<IntervalRecurrencePattern, WeekdayRecurrencePattern>;

    using occurrences_t = RecurrencePattern::occurrences_t;

  private:
    static constexpr auto jsonPatternTypeField{std::string_view{"type"}};
    static constexpr auto jsonPatternField{std::string_view{"pattern"}};

    static constexpr auto jsonFields{std::array<std::string_view, 2>{
        jsonPatternTypeField, jsonPatternField}};

    static constexpr auto jsonIntervalPatternTypeValue{
        std::string_view{"interval"}};
    static constexpr auto jsonWeekdayPatternTypeValue{
        std::string_view{"weekday"}};

  private:
    enum class Error : uint8_t {
        JSONMissingRequiredField,

        JSONFailedToParseIntervalPattern,
        JSONFailedToParseWeekdayPattern,

        JSONUnsupportedPatternType,

        UnsupportedPatternType,
    };

  public:
    [[nodiscard]] static constexpr auto errorMessage(Error error)
        -> std::string {
        switch (error) {
        case Error::JSONMissingRequiredField:
            return "Recurrence: missing required field(s) in JSON";

        case Error::JSONFailedToParseIntervalPattern:
            return "Recurrence: failed to parse IntervalRecurrencePattern from "
                   "JSON";

        case Error::JSONFailedToParseWeekdayPattern:
            return "Recurrence: failed to parse WeekdayRecurrencePattern from "
                   "JSON";

        case Error::JSONUnsupportedPatternType:
            return "Recurrence: unsupported pattern type provided in JSON";

        case Error::UnsupportedPatternType:
            return "Recurrence: invalid object state (unsupported pattern "
                   "type)";
        }
    }

  public:
    enum class PatternType : uint8_t {
        Interval,
        Weekday,
    };

  private:
    pattern_t pattern_;

  public:
    Recurrence(pattern_t pattern);

  public:
    [[nodiscard]] auto getPatternType() const -> PatternType;

    [[nodiscard]] auto getIntervalPattern() const -> IntervalRecurrencePattern;

    [[nodiscard]] auto getWeekdayPattern() const -> WeekdayRecurrencePattern;

  public:
    [[nodiscard]] auto getOccurrencesOnDate(DateTime start, Date date) const
        -> occurrences_t;

  public:
    [[nodiscard]] auto isForDate(DateTime start, Date date) const -> bool;

  private:
    [[nodiscard]] static auto containsAllJSONFields(const nlohmann::json &json)
        -> bool;

  public:
    [[nodiscard]] auto toJSON() const -> nlohmann::json;

    [[nodiscard]] static auto fromJSON(const nlohmann::json &json)
        -> std::expected<Recurrence, Error>;
};
} // namespace hbt::mods::util
