#pragma once

#include <datetime.hpp>
#include <interval_pattern.hpp>
#include <null_pattern.hpp>
#include <opt_datetime.hpp>
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

  private:
    enum class Error : uint8_t {
        UnsupportedPatternType,

        EndBeforeStart,
        StartAfterEnd,
    };

  public:
    [[nodiscard]] static constexpr auto errorMessage(Error error)
        -> std::string {
        switch (error) {
        case Error::UnsupportedPatternType:
            return "Recurrence: invalid object state (unsupported pattern "
                   "type)";

        case Error::EndBeforeStart:
            return "Recurrence: end DateTime cannot appear before "
                   "start DateTime";

        case Error::StartAfterEnd:
            return "Recurrence: start DateTime cannot appear after end "
                   "DateTime";

        default:
            std::unreachable();
        }
    }

  public:
    enum class PatternType : uint8_t {
        Null,
        Interval,
        Weekdays,
    };

  private:
    pattern_t pattern_;
    DateTime startDateTime_;
    OptDateTime endDateTime_;

  public:
    Recurrence(pattern_t pattern, DateTime startDateTime,
               OptDateTime endDateTime);

  public:
    [[nodiscard]] static auto null(DateTime startDateTime = DateTime::now())
        -> Recurrence;

  public:
    [[nodiscard]] auto getDateTimesOfDate(DateTime datetime) const
        -> std::vector<DateTime>;

    [[nodiscard]] auto happensOnDate(DateTime date) const -> bool;

  public:
    [[nodiscard]] auto getPatternType() const -> PatternType;

    [[nodiscard]] auto getStartDateTime() const -> DateTime;

    [[nodiscard]] auto getEndDateTime() const -> OptDateTime;

  public:
    auto setPatternType(pattern_t pattern) -> void;

    auto setStartDateTime(DateTime startDateTime) -> void;

    auto setEndDateTime(OptDateTime endDateTime) -> void;

  public:
    [[nodiscard]] auto isIntervalPattern() const -> bool;

    [[nodiscard]] auto isWeekdaysPattern() const -> bool;

    [[nodiscard]] auto isNullPattern() const -> bool;

  public:
    [[nodiscard]] auto getIntervalPattern() const -> IntervalRecurrencePattern;

    [[nodiscard]] auto getWeekdaysPattern() const -> WeekdaysRecurrencePattern;

  public:
    [[nodiscard]] auto operator==(const Recurrence &recurrence) const
        -> bool = default;

  private:
    struct Validator {
        struct Validated {};

        static auto endAfterStart(OptDateTime end, DateTime start) -> void;

        struct Return {
            [[nodiscard]] static auto endAfterStart(OptDateTime end,
                                                    DateTime start)
                -> OptDateTime;

            [[nodiscard]] static auto startBeforeEnd(DateTime start,
                                                     OptDateTime end)
                -> DateTime;
        };
    };

  public:
    Recurrence(Validator::Validated, pattern_t pattern, DateTime startDateTime,
               OptDateTime endDateTime);

    [[nodiscard]] static auto fromValidated(pattern_t pattern,
                                            DateTime startDateTime,
                                            OptDateTime endDateTime);

  public:
    struct JSON {
        static constexpr auto patternTypeField{std::string_view{"type"}};
        static constexpr auto patternField{std::string_view{"pattern"}};
        static constexpr auto startDateTimeField{std::string_view{"start"}};
        static constexpr auto endDateTimeField{std::string_view{"end"}};

        static constexpr auto fields{std::array<std::string_view, 4>{
            patternTypeField, patternField, startDateTimeField,
            endDateTimeField}};

        static constexpr auto nullPatternTypeValue{std::string_view{"null"}};
        static constexpr auto intervalPatternTypeValue{
            std::string_view{"interval"}};
        static constexpr auto weekdayPatternTypeValue{
            std::string_view{"weekday"}};

        enum class Error : uint8_t {
            MissingRequiredField,

            FailedToParseNullPattern,
            FailedToParseIntervalPattern,
            FailedToParseWeekdayPattern,

            UnsupportedPatternType,

            FailedToParseStartDateTime,
            FailedToParseEndDateTime,

            ParsedEndDateTimeBeforeStartDateTime,
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

            case JSON::Error::FailedToParseNullPattern:
                return "Recurrence::JSON: failed to parse "
                       "NullRecurrencePattern";

            case JSON::Error::FailedToParseIntervalPattern:
                return "Recurrence::JSON: failed to parse "
                       "IntervalRecurrencePattern";

            case JSON::Error::FailedToParseWeekdayPattern:
                return "Recurrence::JSON: failed to parse "
                       "WeekdayRecurrencePattern";

            case JSON::Error::UnsupportedPatternType:
                return "Recurrence::JSON: unsupported pattern type";

            case JSON::Error::FailedToParseStartDateTime:
                return "Recurrence::JSON: failed to parse start DateTime";

            case JSON::Error::FailedToParseEndDateTime:
                return "Recurrence::JSON: failed to parse end OptDateTime";

            case JSON::Error::ParsedEndDateTimeBeforeStartDateTime:
                return "Recurrence::JSON: parsed end DateTime appears after "
                       "start DateTime";

            default:
                std::unreachable();
            }
        }
    };
};
} // namespace hbt::mods::util
