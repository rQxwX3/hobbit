#pragma once

#include <datetime.hpp>
#include <interval_pattern.hpp>
#include <null_pattern.hpp>
#include <weekdays_pattern.hpp>

#include <nlohmann/json.hpp>

#include <array>
#include <expected>
#include <string_view>
#include <variant>

namespace hbt::mods::util {
class Recurrence {
  public:
    using pattern_t =
        std::variant<NullRecurrencePattern, IntervalRecurrencePattern,
                     WeekdaysRecurrencePattern>;

    using occurrences_t = RecurrencePattern::occurrences_t;

  private:
    enum class Error : uint8_t {
        UnsupportedPatternType,
    };

  public:
    [[nodiscard]] static constexpr auto errorMessage(Error error)
        -> std::string {
        switch (error) {
        case Error::UnsupportedPatternType:
            return "Recurrence: invalid object state (unsupported pattern "
                   "type)";

        default:
            std::unreachable();
        }
    }

  public:
    enum class PatternType : uint8_t {
        Interval,
        Weekdays,
        Null,
    };

  private:
    pattern_t pattern_;

  public:
    Recurrence(pattern_t pattern);

  public:
    [[nodiscard]] static auto null() -> Recurrence;

  public:
    [[nodiscard]] auto getPatternType() const -> PatternType;

    [[nodiscard]] auto isIntervalPattern() const -> bool;

    [[nodiscard]] auto isWeekdaysPattern() const -> bool;

    [[nodiscard]] auto isNullPattern() const -> bool;

    [[nodiscard]] auto getIntervalPattern() const -> IntervalRecurrencePattern;

    [[nodiscard]] auto getWeekdaysPattern() const -> WeekdaysRecurrencePattern;

  public:
    [[nodiscard]] auto happensOnDate(DateTime on) const -> bool;

  public:
    [[nodiscard]] auto getOccurrencesOfDate(DateTime datetime) const
        -> occurrences_t;

  public:
    [[nodiscard]] auto operator==(const Recurrence &recurrence) const
        -> bool = default;

  public:
    struct JSON {
        static constexpr auto patternTypeField{std::string_view{"type"}};
        static constexpr auto patternField{std::string_view{"pattern"}};

        static constexpr auto fields{
            std::array<std::string_view, 2>{patternTypeField, patternField}};

        static constexpr auto intervalPatternTypeValue{
            std::string_view{"interval"}};
        static constexpr auto weekdayPatternTypeValue{
            std::string_view{"weekday"}};

        enum class Error : uint8_t {
            MissingRequiredField,

            FailedToParseIntervalPattern,
            FailedToParseWeekdayPattern,

            UnsupportedPatternType,
        };

        [[nodiscard]] static auto containsAllFields(const nlohmann::json &json)
            -> bool;

        [[nodiscard]] static auto encode(const Recurrence &recurrence)
            -> nlohmann::json;

        [[nodiscard]] static auto decode(const nlohmann::json &json)
            -> std::expected<Recurrence, JSON::Error>;

        [[nodiscard]] static constexpr auto errorMessage(Error error)
            -> std::string {
            switch (error) {
            case JSON::Error::MissingRequiredField:
                return "Recurrence::JSON: missing required field(s)";

            case JSON::Error::FailedToParseIntervalPattern:
                return "Recurrence::JSON: failed to parse "
                       "IntervalRecurrencePattern";

            case JSON::Error::FailedToParseWeekdayPattern:
                return "Recurrence::JSON: failed to parse "
                       "WeekdayRecurrencePattern";

            case JSON::Error::UnsupportedPatternType:
                return "Recurrence::JSON: unsupported pattern type";

            default:
                std::unreachable();
            }
        }
    };
};
} // namespace hbt::mods::util
