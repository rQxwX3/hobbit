#pragma once

#include <nlohmann/json.hpp>

#include <datetime/week.hpp>
#include <error.hpp>
#include <json.hpp>

namespace clndr::codec::json::dt {
struct Week : core::codec::json::Base<Week> {
    struct Error : core::err::Base<Error> {
        static constexpr auto className{std::string_view{"JSON::dt::Week"}};

        enum class Code : uint8_t {
            FailedToParseDate,
        };

        [[nodiscard]] static constexpr auto getMessage(Code code)
            -> std::string;
    };

    static_assert(core::err::Concept<Error>);

    enum class Field : size_t {
        count_,
    };

    static constexpr auto fields{
        std::array<std::string_view, static_cast<size_t>(Field::count_)>{}};

    [[nodiscard]] static auto encode(const clndr::dt::Week &week)
        -> nlohmann::json;

    [[nodiscard]] static auto decode(const nlohmann::json &json)
        -> std::expected<clndr::dt::Week, Error::Code>;
};

static_assert(core::codec::json::Concept<Week, clndr::dt::Week>);
} // namespace clndr::codec::json::dt
