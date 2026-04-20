#include <task_data.hpp>

namespace hbt::mods {
TaskData::TaskData(std::string title, startFrom_t startFrom, bool isCompleted,
                   deadline_t deadline)
    : title{std::move(title)}, startFrom{startFrom},
      deadline{std::move(deadline)}, isCompleted{isCompleted} {}

[[nodiscard]] auto TaskData::toJSON() const & -> nlohmann::json {
    nlohmann::json json = {
        {"title", title},
        {"datetime", startFrom.toISO8601String()},
        {"deadline", deadlineToJSON()},
        {"is_completed", isCompleted},
    };

    return json;
}

[[nodiscard]] auto TaskData::fromJSON(const nlohmann::json &json)
    -> std::optional<TaskData> {
    if (!json.contains("title") || !json.contains("is_completed") ||
        !json.contains("datetime") || !json.contains("deadline")) {
        return std::nullopt;
    }

    auto dateTimeFromISO8601{hbt::mods::DateTime::fromISO8601String(
        json["datetime"].get<std::string>())};
    if (!dateTimeFromISO8601.has_value()) {
        return std::nullopt;
    }

    auto deadlineFromJSON{TaskData::deadlineFromJSON(json["deadline"])};
    if (!deadlineFromJSON.has_value()) {
        return std::nullopt;
    }

    return TaskData{json["title"].get<std::string>(),
                    dateTimeFromISO8601.value(),
                    json["is_completed"].get<bool>(), deadlineFromJSON.value()};
}

[[nodiscard]] auto TaskData::deadlineToJSON() const -> nlohmann::json {
    if (std::holds_alternative<std::monostate>(deadline)) {
        return {{"type", "none"}};
    }

    if (std::holds_alternative<hbt::mods::Interval>(deadline)) {
        auto intervalJSON{std::get<hbt::mods::Interval>(deadline).toJSON()};

        return {{"type", "interval"}, {"interval", intervalJSON}};
    }

    auto datetimeISO8601{
        std::get<hbt::mods::DateTime>(deadline).toISO8601String()};

    return {{"type", "datetime"}, {"datetime", datetimeISO8601}};
}

[[nodiscard]] auto TaskData::deadlineFromJSON(const nlohmann::json &json)
    -> std::optional<deadline_t> {
    if (!json.contains("type")) {
        return std::nullopt;
    }

    if (json["type"] == "none") {
        return std::monostate{};
    }

    if (json["type"] == "interval") {
        if (!json.contains("interval")) {
            return std::nullopt;
        }

        return hbt::mods::Interval::fromJSON(json["interval"]);
    }

    if (json["type"] == "datetime") {
        if (!json.contains("datetime")) {
            return std::nullopt;
        }

        return hbt::mods::DateTime::fromISO8601String(json["datetime"]);
    }

    return std::nullopt;
}
} // namespace hbt::mods
