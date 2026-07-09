#pragma once

#include <error.hpp>
#include <json.hpp>
#include <recurrence/weekdays_pattern.hpp>

namespace clndr::codec::json {
struct WeekdaysPattern : core::codec::json::Base<rec::WeekdaysPattern> {
    struct Error : core::err::Base<Error> {
        static constexpr auto className{
            std::string_view{"JSON::WeekdaysPattern"}};

        enum class Code : uint8_t {
            MissingRequiredField,

            FailedToParseFirstWeek,
            FailedToParseInterval,
            FailedToParseWeek,

            FailedToValidateInterval,
            FailedToValidateWeek,
        };

        [[nodiscard]] static constexpr auto getMessage(Code code)
            -> std::string {
            switch (code) {
            case Code::MissingRequiredField:
                return generateMessage("missing required field(s)");

            case Code::FailedToParseInterval:
                return generateMessage("failed to parse Interval");

            case Code::FailedToParseFirstWeek:
                return generateMessage("failed to parse clndr::Week");

            case Code::FailedToParseWeek:
                return generateMessage("failed to parse Week");

            case Code::FailedToValidateInterval:
                return generateMessage("failed to validate parsed Interval");

            case Code::FailedToValidateWeek:
                return generateMessage("failed to validate parsed Week");

            default:
                std::unreachable();
            }
        }
    };

    static_assert(core::err::Concept<Error>);

    enum class Field : size_t { firstWeek, interval, selectedWeekdays, count_ };

    static constexpr auto fields{
        std::array<std::string_view, static_cast<size_t>(Field::count_)>{
            "first_week", "interval", "selected_weekdays"}};

    [[nodiscard]] static auto encode(const rec::WeekdaysPattern &pattern)
        -> nlohmann::json;

    [[nodiscard]] static auto decode(const nlohmann::json &json)
        -> std::expected<rec::WeekdaysPattern, Error::Code>;
};

static_assert(
    core::codec::json::Concept<WeekdaysPattern, rec::WeekdaysPattern>);
}; // namespace clndr::codec::json
