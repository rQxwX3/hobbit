#pragma once

#include <nlohmann/json.hpp>

#include <codec/iso8601/interval.hpp>
#include <datetime/interval.hpp>
#include <error.hpp>
#include <json.hpp>

namespace clndr::codec::json::dt {
struct Interval : core::codec::json::Base<Interval> {
    struct Error : core::err::Base<Error> {
        static constexpr auto className{std::string_view{"JSON::dt::Interval"}};

        enum class Code : uint8_t {
            MissingRequiredField,
            FailedToParseIntervalDuration,
            InvalidMonthHandling,
        };

        [[nodiscard]] static constexpr auto getMessage(Code code)
            -> std::string {
            switch (code) {
            case Code::MissingRequiredField:
                return generateMessage(
                    "provided input doesn't contain required field(s)");

            case Code::FailedToParseIntervalDuration:
                return generateMessage(
                    "failed to parse Interval from provided input");

            case Code::InvalidMonthHandling:
                return generateMessage(
                    "provided input contains invalid MonthHandling");

            default:
                std::unreachable();
            }
        }
    };

    static_assert(core::err::Concept<Error>);

    enum class Field : size_t {
        duration,
        monthHandling,
        count_,
    };

    static constexpr auto fields{
        std::array<std::string_view, static_cast<size_t>(Field::count_)>{
            "duration", "month_handling"}};

    [[nodiscard]] static auto encode(const clndr::dt::Interval &interval)
        -> nlohmann::json;

    [[nodiscard]] static auto decode(const nlohmann::json &json)
        -> std::expected<clndr::dt::Interval, Error::Code>;
};

static_assert(core::codec::json::Concept<Interval, clndr::dt::Interval>);
} // namespace clndr::codec::json::dt
