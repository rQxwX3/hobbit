#pragma once

#include <nlohmann/json.hpp>

#include <error.hpp>
#include <json.hpp>
#include <recurrence/selected_weekdays.hpp>

namespace clndr::codec::json {
struct SelectedWeekdays : core::codec::json::Base<SelectedWeekdays> {
    struct Error : core::err::Base<Error> {
        static constexpr auto className{
            std::string_view{"JSON::SelectedWeekdays"}};

        enum class Code : uint8_t {
            NotString,
            InvalidString,
        };

        [[nodiscard]] static constexpr auto getMessage(Code code)
            -> std::string {
            switch (code) {
            case Code::NotString:
                return "Week: provided JSON doesn't contains a string";

            case Code::InvalidString:
                return "Week: provided JSON contains invalid string";

            default:
                std::unreachable();
            }
        }
    };

    static_assert(core::err::Concept<Error>);

    enum class Field : size_t {
        count_,
    };

    static constexpr auto fields{
        std::array<std::string_view, static_cast<size_t>(Field::count_)>{}};

    [[nodiscard]] static auto
    encode(const rec::SelectedWeekdays &selectedWeekdays) -> nlohmann::json;

    [[nodiscard]] static auto decode(const nlohmann::json &json)
        -> std::expected<rec::SelectedWeekdays, Error::Code>;
};

static_assert(
    core::codec::json::Concept<SelectedWeekdays, rec::SelectedWeekdays>);
} // namespace clndr::codec::json
