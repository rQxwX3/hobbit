#include <singular_task.hpp>

namespace hbt::mods {
SingularTask::SingularTask(TaskData taskData, bool isCompleted)
    : taskData_(std::move(taskData)), isCompleted_{isCompleted} {}

[[nodiscard]] auto SingularTask::getTaskData() const -> TaskData {
    return taskData_;
}

[[nodiscard]] auto SingularTask::getTitle() const -> std::string_view {
    return taskData_.getTitle();
}

[[nodiscard]] auto SingularTask::getDateTime() const -> DateTime {
    return taskData_.getDateTime();
}

[[nodiscard]] auto SingularTask::getDeadline() const -> Deadline {
    return taskData_.getDeadline();
}

[[nodiscard]] auto SingularTask::isCompleted() const -> bool {
    return isCompleted_;
}

auto SingularTask::setTitle(std::string title) -> void {
    try {
        taskData_.setTitle(std::move(title));
    } catch (const std::exception &e) {
        rethrowTaskDataInvalidArgumentException(e);
    }
}

auto SingularTask::setDateTime(DateTime datetime) -> void {
    try {
        taskData_.setDateTime(datetime);
    } catch (const std::exception &e) {
        rethrowTaskDataInvalidArgumentException(e);
    }
}

auto SingularTask::setDeadline(Deadline deadline) -> void {
    try {
        taskData_.setDeadline(deadline);
    } catch (const std::exception &e) {
        rethrowTaskDataInvalidArgumentException(e);
    }
}

auto SingularTask::setCompleted(bool isCompleted) -> void {
    isCompleted_ = isCompleted;
}

[[nodiscard]] auto SingularTask::isForDate(DateTime datetime) const -> bool {
    return getDateTime().getDate() == datetime.getDate();
}

/* ------- JSON ------- */
[[nodiscard]] auto
SingularTask::JSON::containsAllFields(const nlohmann::json &json) -> bool {
    return std::ranges::all_of(fields, [&json](const auto &field) -> bool {
        return json.contains(field);
    });
}

[[nodiscard]] auto SingularTask::JSON::encode(const SingularTask &singularTask)
    -> nlohmann::json {
    return {
        {taskDataField, TaskData::JSON::encode(singularTask.getTaskData())},
        {isCompletedField, singularTask.isCompleted()},
    };
}

[[nodiscard]] auto SingularTask::JSON::decode(const nlohmann::json &json)
    -> std::expected<SingularTask, Error> {
    if (!containsAllFields(json)) {
        return std::unexpected(Error::MissingRequiredField);
    }

    auto taskDataJSON{TaskData::JSON::decode(json[taskDataField])};
    if (!taskDataJSON) {
        return std::unexpected(Error::FailedToParseTaskData);
    }

    return SingularTask(taskDataJSON.value(),
                        json[isCompletedField].get<bool>());
}
} // namespace hbt::mods
