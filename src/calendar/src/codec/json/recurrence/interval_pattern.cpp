#include <codec/json/datetime/interval.hpp>
#include <codec/json/recurrence/interval_pattern.hpp>
#include <recurrence/interval_pattern.hpp>

namespace clndr::codec::json::rec {
[[nodiscard]] auto
IntervalPattern::encode(const clndr::rec::IntervalPattern &pattern)
    -> nlohmann::json {
    return {{getFieldName(Field::interval),
             codec::json::dt::Interval::encode(pattern.getInterval())}};
}

[[nodiscard]] auto IntervalPattern::decode(const nlohmann::json &json)
    -> std::expected<clndr::rec::IntervalPattern, Error::Code> {
    if (!containsAllFields(json)) {
        return std::unexpected(Error::Code::MissingRequiredField);
    }

    auto intervalFromJSON{
        codec::json::dt::Interval::decode(json[getFieldName(Field::interval)])};
    if (!intervalFromJSON) {
        return std::unexpected(Error::Code::FailedToParseInterval);
    }

    try {
        clndr::rec::IntervalPattern::validateInterval(intervalFromJSON.value());
    } catch (std::invalid_argument) {
        return std::unexpected(Error::Code::ParsedIntervalZero);
    }

    return clndr::rec::IntervalPattern(intervalFromJSON.value());
}
}; // namespace clndr::codec::json::rec
