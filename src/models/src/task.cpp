#include <task.hpp>

namespace hbt::mods {
Task::Task(std::string title, hbt::mods::DateTime startFrom,
           deadline_t deadline, bool isCompleted)
    : title_{std::move(title)}, startFrom_{startFrom}, deadline_{deadline},
      isCompleted_{isCompleted} {}

auto Task::setTitle(std::string title) -> void { title_ = std::move(title); }

auto Task::setStartFrom(hbt::mods::DateTime startFrom) -> void {
    if (!deadline_.has_value()) {
        startFrom_ = startFrom;
        return;
    }

    if (startFrom > deadline_) {
        // TODO: handle the error
    }

    startFrom_ = startFrom;
}

auto Task::setDeadline(deadline_t deadline) -> void {
    if (!deadline.has_value()) {
        deadline_ = deadline;
        return;
    }

    if (deadline < startFrom_) {
        // TODO: handle the error
        return;
    }

    deadline_ = deadline;
}

auto Task::setIsCompleted(bool isCompleted) -> void {
    isCompleted_ = isCompleted;
}

[[nodiscard]] auto Task::getTitle() const & -> const std::string & {
    return title_;
}

[[nodiscard]] auto Task::getStartFrom() const -> hbt::mods::DateTime {
    return startFrom_;
}

[[nodiscard]] auto Task::getDeadline() const -> deadline_t { return deadline_; }

[[nodiscard]] auto Task::isCompleted() const -> bool { return isCompleted_; }

[[nodiscard]] auto Task::toJSON() const & -> nlohmann::json {
    nlohmann::json json = {
        {"title", title_},
        {"datetime", startFrom_.toISO8601String()},
        {"deadline", (deadline_.has_value() ? deadline_->toISO8601String()
                                            : zeroDeadlineJSON)},
        {"is_completed", isCompleted_},
    };

    return json;
}

[[nodiscard]] auto Task::toJSON() && -> nlohmann::json {
    nlohmann::json json = {
        {"title", std::move(title_)},
        {"datetime", startFrom_.toISO8601String()},
        {"deadline", (deadline_.has_value() ? deadline_->toISO8601String()
                                            : zeroDeadlineJSON)},
        {"is_completed", isCompleted_},
    };

    return json;
}

[[nodiscard]] auto Task::fromJSON(const nlohmann::json &json)
    -> std::optional<Task> {
    if (!json.contains("title") || !json.contains("is_completed") ||
        !json.contains("datetime") || !json.contains("deadline")) {
        return std::nullopt;
    }

    auto dateTimeFromISO8601{hbt::mods::DateTime::fromISO8601String(
        json["datetime"].get<std::string>())};
    if (!dateTimeFromISO8601.has_value()) {
        return std::nullopt;
    }

    auto deadlineFromISO8601{deadline_t{std::nullopt}};
    if (auto dlISO8601{json["deadline"].get<std::string>()};
        dlISO8601 != zeroDeadlineJSON) {
        deadlineFromISO8601 = hbt::mods::DateTime::fromISO8601String(dlISO8601);
    }

    return Task{json["title"].get<std::string>(), dateTimeFromISO8601.value(),
                deadlineFromISO8601, json["is_completed"].get<bool>()};
}
} // namespace hbt::mods
