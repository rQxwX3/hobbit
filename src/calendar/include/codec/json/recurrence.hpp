#pragma once

#include <error.hpp>
#include <json.hpp>
#include <recurrence/recurrence.hpp>

namespace clndr::codec::json {
struct Recurrence : core::codec::json::Base<Recurrence> {
    struct Error : core::err::Base<Error> {
        static constexpr auto className{std::string_view{"JSON::Recurrence"}};

        enum class Code : uint8_t {
            MissingRequiredField,

            FailedToParseNullPattern,
            FailedToParseIntervalPattern,
            FailedToParseWeekdayPattern,

            UnsupportedPatternType,

            FailedToParseStartDateTime,
            FailedToParseEndDateTime,

            ParsedEndDateTimeBeforeStartDateTime,
        };

        [[nodiscard]] static constexpr auto getMessage(Code code)
            -> std::string {
            switch (code) {
            case Code::MissingRequiredField:
                return "missing required field(s)";

            case Code::FailedToParseNullPattern:
                return "failed to parse "
                       "NullRecurrencePattern";

            case Code::FailedToParseIntervalPattern:
                return "failed to parse IntervalRecurrencePattern";

            case Code::FailedToParseWeekdayPattern:
                return "failed to parse WeekdayRecurrencePattern";

            case Code::UnsupportedPatternType:
                return "unsupported pattern type";

            case Code::FailedToParseStartDateTime:
                return "failed to parse start DateTime";

            case Code::FailedToParseEndDateTime:
                return "failed to parse end OptDateTime";

            case Code::ParsedEndDateTimeBeforeStartDateTime:
                return "parsed end DateTime appears after start DateTime";

            default:
                std::unreachable();
            }
        }
    };

    static_assert(core::err::Concept<Error>);

    enum class Field : size_t { type, pattern, start, end, count_ };

    static constexpr auto fields{
        std::array<std::string_view, static_cast<size_t>(Field::count_)>{
            "type", "pattern", "start", "end"}};

    static constexpr auto nullPatternTypeValue{std::string_view{"null"}};
    static constexpr auto intervalPatternTypeValue{
        std::string_view{"interval"}};
    static constexpr auto weekdayPatternTypeValue{std::string_view{"weekday"}};

    [[nodiscard]] static auto encode(const rec::Recurrence &recurrence)
        -> nlohmann::json;

    [[nodiscard]] static auto decode(const nlohmann::json &json)
        -> std::expected<rec::Recurrence, Error::Code>;
};

static_assert(core::codec::json::Concept<Recurrence, rec::Recurrence>);
}; // namespace clndr::codec::json
