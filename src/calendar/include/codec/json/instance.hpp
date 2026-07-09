#pragma once

#include <error.hpp>
#include <event/instance.hpp>
#include <json.hpp>

namespace clndr::codec::json {
struct Instance : core::codec::json::Base<Instance> {
    struct Error : core::err::Base<Error> {
        static constexpr auto className{std::string_view{"JSON::Instance"}};

        enum class Code : uint8_t {
            MissingRequiredField,
            FailedToParseDateTime,
        };

        [[nodiscard]] static constexpr auto getMessage(Code code)
            -> std::string {
            switch (code) {}
        };

        static_assert(core::err::Concept<Error>);
    };

    enum class Field : size_t {
        uuid,
        templateUUID,
        datetime,
        completed,
        count_
    };

    static constexpr auto fields{
        std::array<std::string_view, static_cast<size_t>(Field::count_)>{
            "uuid", "template_uuid", "datetime", "completed"}};

    [[nodiscard]] static auto encode(const ev::Instance &instance)
        -> nlohmann::json;

    [[nodiscard]] static auto decode(const nlohmann::json &json)
        -> std::expected<ev::Instance, Error::Code>;

    static_assert(core::codec::json::Concept<Instance, ev::Instance>);
};
} // namespace clndr::codec::json
