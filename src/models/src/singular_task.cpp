#include <singular_task.hpp>

namespace hbt::mods {
auto SingularTask::validateDeadline(deadline_t deadline) const -> deadline_t {
    if (!std::holds_alternative<hbt::mods::DateTime>(deadline)) {
        return deadline;
    }

    if (auto datetime{std::get<hbt::mods::DateTime>(deadline)};
        datetime < task_.startFrom) {
        throw std::invalid_argument(std::string{invalidDeadlineError});
    }

    return deadline;
}

auto SingularTask::validateStartFrom(hbt::mods::DateTime startFrom) const
    -> startFrom_t {
    if (std::holds_alternative<hbt::mods::DateTime>(task_.deadline) &&
        startFrom > std::get<hbt::mods::DateTime>(task_.deadline)) {
        throw std::invalid_argument(std::string{invalidStartFromError});
    }

    return startFrom;
}

[[nodiscard]] auto SingularTask::validateTaskData(const TaskData &task) const
    -> TaskData {
    validateDeadline(task.deadline);
    validateStartFrom(task.startFrom);

    return task;
}

SingularTask::SingularTask(const TaskData &task)
    : task_{validateTaskData(task)} {}

auto SingularTask::setTitle(std::string title) -> void {
    task_.title = std::move(title);
}

auto SingularTask::setStartFrom(startFrom_t startFrom) -> void {
    task_.startFrom = validateStartFrom(startFrom);
}

auto SingularTask::setDeadline(deadline_t deadline) -> void {
    task_.deadline = validateDeadline(std::move(deadline));
}

auto SingularTask::setIsCompleted(bool isCompleted) -> void {
    task_.isCompleted = isCompleted;
}

[[nodiscard]] auto SingularTask::getTitle() const & -> const std::string & {
    return task_.title;
}

[[nodiscard]] auto SingularTask::getStartFrom() const -> hbt::mods::DateTime {
    return task_.startFrom;
}

[[nodiscard]] auto SingularTask::getDeadline() const -> deadline_t {
    return task_.deadline;
}

[[nodiscard]] auto SingularTask::isCompleted() const -> bool {
    return task_.isCompleted;
}

[[nodiscard]] auto SingularTask::isForDate(hbt::mods::DateTime datetime) const
    -> bool {
    return DateTime::equalDates(task_.startFrom, datetime);
}

[[nodiscard]] auto SingularTask::hasDeadline() const -> bool {
    return !std::holds_alternative<std::monostate>(task_.deadline);
}

[[nodiscard]] auto SingularTask::toJSON() const & -> nlohmann::json {
    return {{"task", task_.toJSON()}};
}

[[nodiscard]] auto SingularTask::fromJSON(const nlohmann::json &json)
    -> std::optional<SingularTask> {
    if (!json.contains("task")) {
        return std::nullopt;
    }

    auto taskFromJSON{TaskData::fromJSON(json["task"])};
    if (!taskFromJSON.has_value()) {
        return std::nullopt;
    }

    return SingularTask{taskFromJSON.value()};
}
} // namespace hbt::mods
