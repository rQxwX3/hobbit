#include <singular_task.hpp>
#include <uuid.hpp>

namespace hbt::mods {
auto SingularTask::validateDeadline(deadline_t deadline) const -> deadline_t {
    if (!std::holds_alternative<hbt::mods::DateTime>(deadline)) {
        return deadline;
    }

    if (auto datetime{std::get<hbt::mods::DateTime>(deadline)};
        datetime < task_.start) {
        throw std::invalid_argument(std::string{invalidDeadlineError});
    }

    return deadline;
}

auto SingularTask::validateStart(start_t start) const -> start_t {
    if (std::holds_alternative<hbt::mods::DateTime>(task_.deadline) &&
        start > std::get<hbt::mods::DateTime>(task_.deadline)) {
        throw std::invalid_argument(std::string{invalidStartFromError});
    }

    return start;
}

[[nodiscard]] auto SingularTask::validateTaskData(const TaskData &task) const
    -> TaskData {
    validateDeadline(task.deadline);
    validateStart(task.start);

    return task;
}

SingularTask::SingularTask(const TaskData &task)
    : task_{validateTaskData(task)}, uuid_{core::uuid::generateUUID()} {}

SingularTask::SingularTask(uuid_t uuid, const TaskData &task)
    : task_{validateTaskData(task)}, uuid_{std::move(uuid)} {}

auto SingularTask::setTitle(std::string title) -> void {
    task_.title = std::move(title);
}

auto SingularTask::setStart(start_t start) -> void {
    task_.start = validateStart(start);
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

[[nodiscard]] auto SingularTask::getStart() const -> start_t {
    return task_.start;
}

[[nodiscard]] auto SingularTask::getDeadline() const -> deadline_t {
    return task_.deadline;
}

[[nodiscard]] auto SingularTask::isCompleted() const -> bool {
    return task_.isCompleted;
}

[[nodiscard]] auto SingularTask::isForDate(hbt::mods::DateTime datetime) const
    -> bool {
    return DateTime::equalDates(task_.start, datetime);
}

[[nodiscard]] auto SingularTask::hasDeadline() const -> bool {
    return !std::holds_alternative<std::monostate>(task_.deadline);
}

[[nodiscard]] auto SingularTask::toJSON() const & -> nlohmann::json {
    return {{"uuid", uuid_}, {"task", task_.toJSON()}};
}

[[nodiscard]] auto SingularTask::fromJSON(const nlohmann::json &json)
    -> std::optional<SingularTask> {
    if (!json.contains("uuid") || !json.contains("task")) {
        return std::nullopt;
    }

    auto taskFromJSON{TaskData::fromJSON(json["task"])};
    if (!taskFromJSON.has_value()) {
        return std::nullopt;
    }

    return SingularTask{json["uuid"].get<std::string>(), taskFromJSON.value()};
}
} // namespace hbt::mods
