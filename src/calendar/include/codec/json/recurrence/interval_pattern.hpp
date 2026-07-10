#pragma once

#include <error.hpp>
#include <json.hpp>
#include <recurrence/interval_pattern.hpp>

namespace clndr::codec::json::rec {
struct IntervalPattern : core::codec::json::Base<rec::IntervalPattern> {
    struct Error : core::err::Base<Error> {
        static constexpr auto className{
            std::string_view{"JSON::rec::IntervalPattern"}};

        enum class Code : uint8_t {
            MissingRequiredField,

            FailedToParseInterval,
            ParsedIntervalZero,
        };

        [[nodiscard]] static constexpr auto getMessage(Code code)
            -> std::string {
            switch (code) {
            case Code::MissingRequiredField:
                return generateMessage("missing required field(s)");

            case Code::FailedToParseInterval:
                return generateMessage("failed to parse Interval");

            case Code::ParsedIntervalZero:
                return generateMessage("parsed Interval is a zero-interval");

            default:
                std::unreachable();
            }
        }
    };

    static_assert(core::err::Concept<Error>);

    enum class Field : size_t { interval, count_ };

    static constexpr auto fields{
        std::array<std::string_view, static_cast<size_t>(Field::count_)>{
            "interval"}};

    [[nodiscard]] static auto encode(const clndr::rec::IntervalPattern &pattern)
        -> nlohmann::json;

    [[nodiscard]] static auto decode(const nlohmann::json &json)
        -> std::expected<clndr::rec::IntervalPattern, Error::Code>;
};

static_assert(
    core::codec::json::Concept<IntervalPattern, clndr::rec::IntervalPattern>);
}; // namespace clndr::codec::json::rec
