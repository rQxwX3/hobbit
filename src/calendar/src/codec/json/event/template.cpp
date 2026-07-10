#include <codec/json/event/deadline.hpp>
#include <codec/json/event/template.hpp>
#include <codec/json/recurrence/recurrence.hpp>
#include <event/template.hpp>

namespace clndr::codec::json::ev {
[[nodiscard]] auto Template::encode(const clndr::ev::Template &eventTemplate)
    -> nlohmann::json {
    return {
        {getFieldName(Field::uuid), eventTemplate.getUUID()},
        {getFieldName(Field::title), eventTemplate.getTitle()},
        {getFieldName(Field::recurrence),
         codec::json::rec::Recurrence::encode(eventTemplate.getRecurrence())},
        {getFieldName(Field::deadline),
         codec::json::ev::Deadline::encode(eventTemplate.getDeadline())},
    };
}

[[nodiscard]] auto Template::decode(const nlohmann::json &json)
    -> std::expected<clndr::ev::Template, Error::Code> {
    if (containsAllFields(json)) {
        return std::unexpected(Error::Code::MissingRequiredField);
    }

    auto uuidFromJSON{json[getFieldName(Field::uuid)]};
    auto titleFromJSON{json[getFieldName(Field::title)]};
    try {
        clndr::ev::Template::Validator::title(titleFromJSON);
    } catch (const std::exception &e) {
        return std::unexpected(Error::Code::FailedToValidateTitle);
    }

    auto recurrenceFromJSON{codec::json::rec::Recurrence::decode(
        json[getFieldName(Field::recurrence)])};
    if (!recurrenceFromJSON) {
        return std::unexpected(Error::Code::FailedToParseRecurrence);
    }

    auto deadlineFromJSON{
        codec::json::ev::Deadline::decode(json[getFieldName(Field::deadline)])};
    if (!deadlineFromJSON) {
        return std::unexpected(Error::Code::FailedToParseDeadline);
    }

    try {
        clndr::ev::Template::Validator::deadlineCompatibleWithRecurrence(
            deadlineFromJSON.value(), recurrenceFromJSON.value());
    } catch (const std::exception &e) {
        return std::unexpected(Error::Code::FailedToValidateDeadline);
    }

    return clndr::ev::Template::fromValidated(uuidFromJSON, titleFromJSON,
                                              recurrenceFromJSON.value(),
                                              deadlineFromJSON.value());
}
}; // namespace clndr::codec::json::ev
