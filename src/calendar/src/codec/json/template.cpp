#include <codec/json/deadline.hpp>
#include <codec/json/recurrence.hpp>
#include <codec/json/template.hpp>
#include <event/template.hpp>

namespace clndr::codec::json {
[[nodiscard]] auto Template::encode(const ev::Template &eventTemplate)
    -> nlohmann::json {
    return {
        {getFieldName(Field::uuid), eventTemplate.getUUID()},
        {getFieldName(Field::title), eventTemplate.getTitle()},
        {getFieldName(Field::recurrence),
         codec::json::Recurrence::encode(eventTemplate.getRecurrence())},
        {getFieldName(Field::deadline),
         codec::json::Deadline::encode(eventTemplate.getDeadline())},
    };
}

[[nodiscard]] auto Template::decode(const nlohmann::json &json)
    -> std::expected<ev::Template, Error::Code> {
    if (containsAllFields(json)) {
        return std::unexpected(Error::Code::MissingRequiredField);
    }

    auto uuidFromJSON{json[getFieldName(Field::uuid)]};
    auto titleFromJSON{json[getFieldName(Field::title)]};
    try {
        ev::Template::Validator::title(titleFromJSON);
    } catch (const std::exception &e) {
        return std::unexpected(Error::Code::FailedToValidateTitle);
    }

    auto recurrenceFromJSON{
        codec::json::Recurrence::decode(json[getFieldName(Field::recurrence)])};
    if (!recurrenceFromJSON) {
        return std::unexpected(Error::Code::FailedToParseRecurrence);
    }

    auto deadlineFromJSON{
        codec::json::Deadline::decode(json[getFieldName(Field::deadline)])};
    if (!deadlineFromJSON) {
        return std::unexpected(Error::Code::FailedToParseDeadline);
    }

    try {
        ev::Template::Validator::deadlineCompatibleWithRecurrence(
            deadlineFromJSON.value(), recurrenceFromJSON.value());
    } catch (const std::exception &e) {
        return std::unexpected(Error::FailedToValidateDeadline);
    }

    return ev::Template::fromValidated(uuidFromJSON, titleFromJSON,
                                       recurrenceFromJSON.value(),
                                       deadlineFromJSON.value());
}
}; // namespace clndr::codec::json
