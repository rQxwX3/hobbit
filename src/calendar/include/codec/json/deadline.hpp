#pragma once

#include <error.hpp>
#include <event/deadline.hpp>
#include <json.hpp>

namespace clndr::codec::json {
struct Deadline : core::codec::json::Base<Deadline> {
    struct Error : core::err::Base<Error> {
        static constexpr auto className{std::string_view{"JSON::Deadline"}};

        enum class Code : uint8_t {
            MissingRequiredTypeField,

            MissingRequiredIntervalField,
            FailedToParseInterval,

            MissingRequiredDateTimeField,
            FailedToParseDateTime,

            UnsupportedType,
        };

        [[nodiscard]] static constexpr auto getMessage(Code code)
            -> std::string {
            switch (code) {
            case Code::MissingRequiredTypeField:
                return "missing required Type field";

            case Code::MissingRequiredIntervalField:
                return "missing required Interval field";

            case Code::FailedToParseInterval:
                return "failed to parse Interval";

            case Code::MissingRequiredDateTimeField:
                return "missing required DateTime field";

            case Code::FailedToParseDateTime:
                return "failed to parse DateTime";

            case Code::UnsupportedType:
                return "provided Type is not supported";

            default:
                std::unreachable();
            }
        }
    };

    static_assert(core::err::Concept<Error>);

    enum class Field : size_t { type, interval, datetime, count_ };

    static constexpr auto fields{
        std::array<std::string_view, 3>{"type", "interval", "datetime"}};

    static constexpr auto typeIntervalValue{std::string_view{"interval"}};
    static constexpr auto typeDateTimeValue{std::string_view{"datetime"}};
    static constexpr auto typeNullValue{std::string_view{"null"}};

    [[nodiscard]] static auto encode(const ev::Deadline &deadline)
        -> nlohmann::json;

    [[nodiscard]] static auto decode(const nlohmann::json &json)
        -> std::expected<ev::Deadline, Error::Code>;
};

static_assert(core::codec::json::Concept<Deadline, ev::Deadline>);
}; // namespace clndr::codec::json
