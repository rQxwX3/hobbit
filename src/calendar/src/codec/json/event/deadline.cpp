#include <codec/iso8601/datetime.hpp>
#include <codec/json/datetime/interval.hpp>
#include <codec/json/event/deadline.hpp>

namespace clndr::codec::json::ev {
[[nodiscard]] auto Deadline::encode(const clndr::ev::Deadline &deadline)
    -> nlohmann::json {
    const auto type{deadline.getUnderlyingType()};

    if (std::holds_alternative<clndr::dt::Interval>(type)) {
        auto intervalJSON =
            json::dt::Interval::encode(std::get<clndr::dt::Interval>(type));

        return {{getFieldName(Field::type), typeIntervalValue},
                {getFieldName(Field::interval), intervalJSON}};
    }

    if (std::holds_alternative<clndr::dt::DateTime>(type)) {
        auto datetimeISO8601{
            iso8601::DateTime::encode(std::get<clndr::dt::DateTime>(type))};
        return {{getFieldName(Field::type), typeDateTimeValue},
                {getFieldName(Field::datetime), datetimeISO8601}};
    }

    return {{getFieldName(Field::type), typeNullValue}};
}

[[nodiscard]] auto Deadline::decode(const nlohmann::json &json)
    -> std::expected<clndr::ev::Deadline, Error::Code> {
    if (!json.contains(getFieldName(Field::type))) {
        return std::unexpected(Error::Code::MissingRequiredTypeField);
    }

    if (json[getFieldName(Field::type)] == typeIntervalValue) {
        if (!json.contains(getFieldName(Field::interval))) {
            return std::unexpected(Error::Code::MissingRequiredIntervalField);
        }

        auto intervalFromJSON{
            json::dt::Interval::decode(json[getFieldName(Field::interval)])};
        if (!intervalFromJSON) {
            return std::unexpected(Error::Code::FailedToParseInterval);
        }

        return clndr::ev::Deadline{intervalFromJSON.value()};
    }

    if (json[getFieldName(Field::type)] == typeDateTimeValue) {
        if (!json.contains(getFieldName(Field::datetime))) {
            return std::unexpected(Error::Code::MissingRequiredDateTimeField);
        }

        auto dateTimeFromJSON{
            iso8601::DateTime::decode(json[getFieldName(Field::datetime)])};
        if (!dateTimeFromJSON) {
            return std::unexpected(Error::Code::FailedToParseDateTime);
        }

        return clndr::ev::Deadline{dateTimeFromJSON.value()};
    }

    if (json[getFieldName(Field::type)] == typeNullValue) {
        return clndr::ev::Deadline(std::monostate());
    }

    return std::unexpected(Error::Code::UnsupportedType);
}
}; // namespace clndr::codec::json::ev
