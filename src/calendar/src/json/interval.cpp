#include <nlohmann/json.hpp>

#include <datetime/interval.hpp>
#include <error.hpp>
#include <json.hpp>

namespace clndr::json {
struct Interval : core::json::Base<Interval> {
    struct Error : core::err::Base<Error> {
        static constexpr auto className{std::string_view{"JSON::Interval"}};

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
        std::array<std::string_view, 2>{"duration", "month_handling"}};

    [[nodiscard]] static auto encode(const dt::Interval &interval)
        -> nlohmann::json {
        return {
            {getFieldName(Field::duration), interval.toISO8601String()},
            {getFieldName(Field::monthHandling), interval.getMonthHandling()}};
    }

    [[nodiscard]] static auto decode(const nlohmann::json &json)
        -> std::expected<dt::Interval, Error::Code> {
        if (!containsAllFields(json)) {
            return std::unexpected(Error::Code::MissingRequiredField);
        }

        auto intervalFromISO8601{dt::Interval::fromISO8601String(
            json[static_cast<size_t>(Field::duration)].get<std::string>())};
        if (!intervalFromISO8601) {
            return std::unexpected(Error::Code::FailedToParseIntervalDuration);
        }

        auto monthHandlingFromJSON{
            json[static_cast<size_t>(Field::monthHandling)]
                .get<dt::Interval::MonthHandling>()};

        try {
            intervalFromISO8601->setMonthHandling(monthHandlingFromJSON);
        } catch (std::invalid_argument) {
            return std::unexpected(Error::Code::InvalidMonthHandling);
        }

        return intervalFromISO8601.value();
    }
};

static_assert(core::json::Concept<Interval, dt::Interval>);
} // namespace clndr::json
