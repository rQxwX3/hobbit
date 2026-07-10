#include <nlohmann/json.hpp>

#include <codec/iso8601/interval.hpp>
#include <codec/json/datetime/interval.hpp>
#include <datetime/interval.hpp>
#include <error.hpp>
#include <json.hpp>

namespace clndr::codec::json::dt {
[[nodiscard]] auto Interval::encode(const clndr::dt::Interval &interval)
    -> nlohmann::json {
    return {{getFieldName(Field::duration),
             codec::iso8601::Interval::encode(interval)},
            {getFieldName(Field::monthHandling), interval.getMonthHandling()}};
}

[[nodiscard]] auto Interval::decode(const nlohmann::json &json)
    -> std::expected<clndr::dt::Interval, Error::Code> {
    if (!containsAllFields(json)) {
        return std::unexpected(Error::Code::MissingRequiredField);
    }

    auto intervalFromISO8601{codec::iso8601::Interval::decode(
        json[getFieldName(Field::duration)].get<std::string>())};
    if (!intervalFromISO8601) {
        return std::unexpected(Error::Code::FailedToParseIntervalDuration);
    }

    auto monthHandlingFromJSON{json[getFieldName(Field::monthHandling)]
                                   .get<clndr::dt::Interval::MonthHandling>()};

    try {
        intervalFromISO8601->setMonthHandling(monthHandlingFromJSON);
    } catch (std::invalid_argument) {
        return std::unexpected(Error::Code::InvalidMonthHandling);
    }

    return intervalFromISO8601.value();
}
} // namespace clndr::codec::json::dt
