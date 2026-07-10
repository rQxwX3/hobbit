#pragma once

#include <error.hpp>
#include <json.hpp>
#include <recurrence/null_pattern.hpp>

namespace clndr::codec::json::rec {
struct NullPattern : core::codec::json::Base<rec::NullPattern> {
    struct Error : core::err::Base<Error> {
        static constexpr auto className{
            std::string_view{"JSON::rec::NullPattern"}};

        enum class Code : uint8_t {
            FailedToParse,
        };

        [[nodiscard]] static constexpr auto getMessage(Code code)
            -> std::string {
            switch (code) {
            case Code::FailedToParse:
                return generateMessage("failed to parse");

            default:
                std::unreachable();
            }
        }
    };

    static_assert(core::err::Concept<Error>);

    enum class Field : size_t { count_ };

    static constexpr auto value{std::string_view("null_pattern")};

    static constexpr auto fields{
        std::array<std::string_view, static_cast<size_t>(Field::count_)>{}};

    [[nodiscard]] static auto encode(const clndr::rec::NullPattern &pattern)
        -> nlohmann::json;

    [[nodiscard]] static auto decode(const nlohmann::json &json)
        -> std::expected<clndr::rec::NullPattern, Error::Code>;
};

static_assert(core::codec::json::Concept<NullPattern, clndr::rec::NullPattern>);
}; // namespace clndr::codec::json::rec
