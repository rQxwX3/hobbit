#include <task_data.hpp>

namespace hbt::mods {
TaskData::TaskData(std::string title, DateTime datetime, bool completed,
                   deadline_t deadline)
    : title{std::move(title)}, datetime{datetime},
      deadline{std::move(deadline)}, completed{completed} {}

[[nodiscard]] auto TaskData::toJSON() const & -> nlohmann::json {
    nlohmann::json json = {
        {jsonTitleField, title},
        {jsonDateTimeField, datetime.toISO8601String()},
        {jsonDeadlineField,
         (deadline.has_value() ? deadline->toJSON() : jsonNullDeadlineValue)},
        {jsonCompletedField, completed},
    };

    return json;
}

[[nodiscard]] auto TaskData::deadlineFromJSON(const nlohmann::json &json)
    -> std::expected<deadline_t, Error> {
    if (json[jsonDeadlineField] == jsonNullDeadlineValue) {
        return std::nullopt;
    }

    auto deadlineFromJSON{Deadline::fromJSON(json[jsonDeadlineField])};
    if (!deadlineFromJSON) {
        return std::unexpected(Error::JSONFailedToParseDeadline);
    }

    return deadlineFromJSON.value();
}

[[nodiscard]] auto TaskData::containsAllJSONFields(const nlohmann::json &json)
    -> bool {
    for (auto field : jsonFields) {
        if (!json.contains(field)) {
            return false;
        }
    }

    return true;
}

[[nodiscard]] auto TaskData::fromJSON(const nlohmann::json &json)
    -> std::expected<TaskData, Error> {
    if (!containsAllJSONFields(json)) {
        return std::unexpected(Error::JSONMissingRequiredField);
    }

    auto dateTimeFromISO8601{DateTime::fromISO8601String(
        json[jsonDateTimeField].get<std::string>())};
    if (!dateTimeFromISO8601) {
        return std::unexpected(Error::JSONFailedToParseDateTime);
    }

    auto deadline{TaskData::deadlineFromJSON(json[jsonDeadlineField])};
    if (!deadline) {
        return std::unexpected(deadline.error());
    }

    return TaskData{json[jsonTitleField].get<std::string>(),
                    dateTimeFromISO8601.value(),
                    json[jsonCompletedField].get<bool>(), deadline.value()};
}
} // namespace hbt::mods
