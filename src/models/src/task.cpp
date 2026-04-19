#include <task.hpp>

namespace hbt::mods {
Task::Task(std::string title) : title_{std::move(title)}, isCompleted_{false} {}

Task::Task(std::string title, bool isCompleted)
    : title_{std::move(title)}, isCompleted_{isCompleted} {}

auto Task::setTitle(std::string title) -> void { title_ = std::move(title); }

auto Task::setIsCompleted(bool isCompleted) -> void {
    isCompleted_ = isCompleted;
}

[[nodiscard]] auto Task::getTitle() const & -> const std::string & {
    return title_;
}

[[nodiscard]] auto Task::isCompleted() const -> bool { return isCompleted_; }

[[nodiscard]] auto Task::toJSON() const & -> nlohmann::json {
    auto occurrencesJSON = nlohmann::json::array();

    nlohmann::json json = {
        {"title", title_},
        {"is_completed", isCompleted_},
    };

    return json;
}

[[nodiscard]] auto Task::toJSON() && -> nlohmann::json {
    nlohmann::json json = {
        {"title", std::move(title_)},
        {"is_completed", isCompleted_},
    };

    return json;
}

[[nodiscard]] auto Task::fromJSON(const nlohmann::json &json)
    -> std::optional<Task> {
    if (!json.contains("title") || !json.contains("is_completed")) {
        return std::nullopt;
    }

    return Task{json["title"].get<std::string>(),
                json["is_completed"].get<bool>()};
}
} // namespace hbt::mods
