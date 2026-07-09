#include <codec/json/interval.hpp>
#include <codec/json/interval_pattern.hpp>
#include <recurrence/interval_pattern.hpp>

namespace clndr::codec::json {
[[nodiscard]] auto IntervalPattern::encode(const rec::IntervalPattern &pattern)
    -> nlohmann::json {
    return {{getFieldName(Field::interval),
             codec::json::Interval::encode(pattern.getInterval())}};
}

[[nodiscard]] auto IntervalPattern::decode(const nlohmann::json &json)
    -> std::expected<rec::IntervalPattern, Error::Code> {
    if (!containsAllFields(json)) {
        return std::unexpected(Error::Code::MissingRequiredField);
    }

    auto intervalFromJSON{
        codec::json::Interval::decode(json[getFieldName(Field::interval)])};
    if (!intervalFromJSON) {
        return std::unexpected(Error::Code::FailedToParseInterval);
    }

    try {
        rec::IntervalPattern::validateInterval(intervalFromJSON.value());
    } catch (std::invalid_argument) {
        return std::unexpected(Error::Code::ParsedIntervalZero);
    }

    return rec::IntervalPattern(intervalFromJSON.value());
}
}; // namespace clndr::codec::json
