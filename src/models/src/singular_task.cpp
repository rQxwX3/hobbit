#include <singular_task.hpp>

namespace hbt::mods {
SingularTask::SingularTask(TaskData task) : task_(std::move(task)) {}

auto SingularTask::setTitle(std::string title) -> void {
    task_.setTitle(std::move(title));
}

auto SingularTask::setDateTime(datetime_t datetime) -> void {
    task_.setDateTime(datetime);
}

auto SingularTask::setDeadline(deadline_t deadline) -> void {
    task_.setDeadline(std::move(deadline));
}

auto SingularTask::setCompleted(bool completed) -> void {
    task_.setCompleted(completed);
}

[[nodiscard]] auto SingularTask::getTitle() const -> std::string_view {
    return task_.getTitle();
}

[[nodiscard]] auto SingularTask::getDateTime() const -> datetime_t {
    return task_.getDateTime();
}

[[nodiscard]] auto SingularTask::getDeadline() const -> deadline_t {
    return task_.getDeadline();
}

[[nodiscard]] auto SingularTask::isCompleted() const -> bool {
    return task_.isCompleted();
}

[[nodiscard]] auto SingularTask::isForDate(hbt::mods::Date date) const -> bool {
    return getDateTime().getDate() == date;
}

[[nodiscard]] auto SingularTask::hasDeadline() const -> bool {
    return getDeadline().has_value();
}

[[nodiscard]] auto
SingularTask::containsAllJSONFields(const nlohmann::json &json) -> bool {
    return std::ranges::all_of(jsonFields, [&json](const auto &field) -> bool {
        return json.contains(field);
    });
}

[[nodiscard]] auto SingularTask::toJSON() const & -> nlohmann::json {
    return {{jsonTaskField, task_.toJSON()}};
}

[[nodiscard]] auto SingularTask::fromJSON(const nlohmann::json &json)
    -> std::expected<SingularTask, Error> {
    if (!containsAllJSONFields(json)) {
        return std::unexpected(Error::JSONMissingRequiredField);
    }

    auto taskFromJSON{TaskData::fromJSON(json[jsonTaskField])};
    if (!taskFromJSON) {
        return std::unexpected(Error::JSONFailedToParseTaskData);
    }

    return SingularTask{taskFromJSON.value()};
}
} // namespace hbt::mods
