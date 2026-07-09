#pragma once

#include <error.hpp>
#include <event/template.hpp>
#include <json.hpp>

namespace clndr::codec::json {
struct Template : core::codec::json::Base<Template> {
    struct Error : core::err::Base<Error> {
        static constexpr auto className{std::string_view{"JSON::Template"}};

        enum class Code : uint8_t {
            MissingRequiredField,

            FailedToParseRecurrence,
            FailedToParseDeadline,

            FailedToValidateTitle,
            FailedToValidateDeadline,
        };

        [[nodiscard]] static constexpr auto getMessage(Code code)
            -> std::string {
            switch (code) {
            case Code::MissingRequiredField:
                return generateMessage(
                    "provided input misses one or more required fields");

            case Code::FailedToParseRecurrence:
                return generateMessage("failed to parse Recurrence");

            case Code::FailedToParseDeadline:
                return generateMessage("failed to parse Deadline");

            case Code::FailedToValidateTitle:
                return generateMessage("provided input contains invalid title");

            case Code::FailedToValidateDeadline:
                return generateMessage("provided input contains deadline "
                                       "incompatible with its recurrence");

            default:
                std::unreachable();
            }
        };

        static_assert(core::err::Concept<Error>);
    };

    enum class Field : size_t { uuid, title, recurrence, deadline, count_ };

    static constexpr auto fields{
        std::array<std::string_view, static_cast<size_t>(Field::count_)>{
            "uuid", "title", "deadline", "recurrence"}};

    [[nodiscard]] static auto encode(const ev::Template &eventTemplate)
        -> nlohmann::json;

    [[nodiscard]] static auto decode(const nlohmann::json &json)
        -> std::expected<ev::Template, Error::Code>;

    static_assert(core::codec::json::Concept<Template, ev::Template>);
};
} // namespace clndr::codec::json
