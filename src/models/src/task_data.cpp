#include <task_data.hpp>

namespace hbt::mods {
[[nodiscard]] auto TaskData::validateTitle(const std::string &title)
    -> std::string {
    if (title.empty()) {
        throw std::invalid_argument(errorMessage(Error::EmptyTitle));
    }

    return title;
}

TaskData::TaskData(std::string title, bool completed)
    : title_{std::move(validateTitle(title))}, completed_{completed} {}

[[nodiscard]] auto TaskData::getTitle() const -> std::string_view {
    return title_;
}

[[nodiscard]] auto TaskData::getCompleted() const -> bool { return completed_; }

auto TaskData::setTitle(std::string title) -> void {
    title_ = std::move(title);
}

auto TaskData::setCompleted(bool completed) -> void { completed_ = completed; }

[[nodiscard]] auto TaskData::toJSON() const & -> nlohmann::json {
    nlohmann::json json = {
        {jsonTitleField, title_},
        {jsonCompletedField, completed_},
    };

    return json;
}

[[nodiscard]] auto TaskData::containsAllJSONFields(const nlohmann::json &json)
    -> bool {
    return std::ranges::all_of(jsonFields, [json](const auto &field) -> bool {
        return json.contains(field);
    });
}

[[nodiscard]] auto TaskData::fromJSON(const nlohmann::json &json)
    -> std::expected<TaskData, Error> {
    if (!containsAllJSONFields(json)) {
        return std::unexpected(Error::JSONMissingRequiredField);
    }

    auto titleFromJSON{json[jsonTitleField].get<std::string>()};
    try {
        validateTitle(titleFromJSON);
    } catch (std::invalid_argument) {
        return std::unexpected(Error::JSONEmptyTitle);
    }

    return TaskData{titleFromJSON, json[jsonCompletedField].get<bool>()};
}
} // namespace hbt::mods
