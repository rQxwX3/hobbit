#include <codec/iso8601/datetime.hpp>
#include <codec/json/deadline.hpp>
#include <codec/json/interval.hpp>

namespace clndr::codec::json {
[[nodiscard]] auto Deadline::encode(const ev::Deadline &deadline)
    -> nlohmann::json {
    const auto type{deadline.getUnderlyingType()};

    if (std::holds_alternative<dt::Interval>(type)) {
        auto intervalJSON =
            json::Interval::encode(std::get<dt::Interval>(type));

        return {{getFieldName(Field::type), typeIntervalValue},
                {getFieldName(Field::interval), intervalJSON}};
    }

    if (std::holds_alternative<dt::DateTime>(type)) {
        auto datetimeISO8601{
            iso8601::DateTime::encode(std::get<dt::DateTime>(type))};
        return {{getFieldName(Field::type), typeDateTimeValue},
                {getFieldName(Field::datetime), datetimeISO8601}};
    }

    return {{getFieldName(Field::type), typeNullValue}};
}

[[nodiscard]] auto Deadline::decode(const nlohmann::json &json)
    -> std::expected<ev::Deadline, Error::Code> {
    if (!json.contains(getFieldName(Field::type))) {
        return std::unexpected(Error::Code::MissingRequiredTypeField);
    }

    if (json[getFieldName(Field::type)] == typeIntervalValue) {
        if (!json.contains(getFieldName(Field::interval))) {
            return std::unexpected(Error::Code::MissingRequiredIntervalField);
        }

        auto intervalFromJSON{
            json::Interval::decode(json[getFieldName(Field::interval)])};
        if (!intervalFromJSON) {
            return std::unexpected(Error::Code::FailedToParseInterval);
        }

        return Deadline{intervalFromJSON.value()};
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

        return Deadline{dateTimeFromJSON.value()};
    }

    if (json[getFieldName(Field::type)] == typeNullValue) {
        return Deadline(std::monostate());
    }

    return std::unexpected(Error::Code::UnsupportedType);
}
}; // namespace clndr::codec::json
