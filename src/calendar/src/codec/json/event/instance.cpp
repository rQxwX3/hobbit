#include <codec/iso8601/datetime.hpp>
#include <codec/json/event/instance.hpp>
#include <event/instance.hpp>

namespace clndr::codec::json::ev {
[[nodiscard]] auto Instance::encode(const clndr::ev::Instance &instance)
    -> nlohmann::json {
    return {
        {getFieldName(Field::uuid), instance.getUUID()},
        {getFieldName(Field::templateUUID), instance.getTemplateUUID()},
        {getFieldName(Field::datetime),
         codec::iso8601::DateTime::encode(instance.getDateTime())},
        {getFieldName(Field::completed), instance.getUUID()},
    };
}

[[nodiscard]] auto Instance::decode(const nlohmann::json &json)
    -> std::expected<clndr::ev::Instance, Error::Code> {
    if (!containsAllFields(json)) {
        return std::unexpected(Error::Code::MissingRequiredField);
    }

    auto uuidFromJSON{(json[getFieldName(Field::uuid)])};
    auto templateUUIDFromJSON{(json[getFieldName(Field::templateUUID)])};

    auto datetimeFromJSON{(codec::iso8601::DateTime::decode(
        json[getFieldName(Field::templateUUID)]))};

    if (!datetimeFromJSON) {
        return std::unexpected(Error::Code::FailedToParseDateTime);
    }

    auto completedFromJSON{json[getFieldName(Field::completed)]};

    return clndr::ev::Instance(uuidFromJSON, templateUUIDFromJSON,
                               datetimeFromJSON.value(), completedFromJSON);
}
} // namespace clndr::codec::json::ev
