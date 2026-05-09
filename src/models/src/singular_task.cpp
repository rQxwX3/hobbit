#include <singular_task.hpp>

namespace hbt::mods {
auto SingularTask::validateDateTime(DateTime datetime) -> DateTime {
    if (deadline_.isDateTime() && datetime >= deadline_.getDateTime()) {
        throw std::invalid_argument(errorMessage(Error::InvalidDateTime));
    }

    return datetime;
}

auto SingularTask::validateDeadline(Deadline deadline) -> Deadline {
    if (deadline.isDateTime() && datetime_ >= deadline.getDateTime()) {
        throw std::invalid_argument(errorMessage(Error::InvalidDeadline));
    }

    return deadline;
}

auto SingularTask::validateDateTimeDeadline(DateTime datetime,
                                            Deadline deadline) -> void {
    if (deadline.isDateTime() && datetime >= deadline.getDateTime()) {
        throw std::invalid_argument(
            errorMessage(Error::JSONInvalidDateTimeDeadline));
    }
}

SingularTask::SingularTask(TaskData task, DateTime datetime, Deadline deadline)
    : task_(std::move(task)), datetime_{datetime},
      deadline_{validateDeadline(deadline)} {}

auto SingularTask::setTitle(std::string title) -> void {
    task_.setTitle(std::move(title));
}

auto SingularTask::setDateTime(DateTime datetime) -> void {
    datetime_ = validateDateTime(datetime);
}

auto SingularTask::setDeadline(Deadline deadline) -> void {
    deadline_ = validateDeadline(deadline);
}

auto SingularTask::setCompleted(bool completed) -> void {
    task_.setCompleted(completed);
}

[[nodiscard]] auto SingularTask::getTitle() const -> std::string_view {
    return task_.getTitle();
}

[[nodiscard]] auto SingularTask::isCompleted() const -> bool {
    return task_.isCompleted();
}

[[nodiscard]] auto SingularTask::getDateTime() const -> DateTime {
    return datetime_;
}

[[nodiscard]] auto SingularTask::getDeadline() const -> Deadline {
    return deadline_;
}

[[nodiscard]] auto SingularTask::isForDate(DateTime datetime) const -> bool {
    return datetime_.getDate() == datetime.getDate();
}

[[nodiscard]] auto SingularTask::hasDeadline() const -> bool {
    return !deadline_.isNull();
}

[[nodiscard]] auto SingularTask::operator==(const SingularTask &other) const
    -> bool = default;

[[nodiscard]] auto
SingularTask::containsAllJSONFields(const nlohmann::json &json) -> bool {
    return std::ranges::all_of(jsonFields, [&json](const auto &field) -> bool {
        return json.contains(field);
    });
}

[[nodiscard]] auto SingularTask::toJSON() const & -> nlohmann::json {
    return {{jsonTaskField, task_.toJSON()},
            {jsonDateTimeField, datetime_.toISO8601String()},
            {jsonDeadlineField, deadline_.toJSON()}};
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

    auto datetimeFromISO8601{DateTime::fromISO8601String(
        json[jsonDateTimeField].get<std::string>())};
    if (!datetimeFromISO8601) {
        return std::unexpected(Error::JSONFailedToParseDateTime);
    }

    auto deadlineFromJSON{(Deadline::fromJSON(json[jsonDeadlineField]))};
    if (!deadlineFromJSON) {
        return std::unexpected(Error::JSONFailedToParseDeadline);
    }

    try {
        validateDateTimeDeadline(datetimeFromISO8601.value(),
                                 deadlineFromJSON.value());
    } catch (std::invalid_argument) {
        return std::unexpected(Error::JSONInvalidDateTimeDeadline);
    }

    return SingularTask(taskFromJSON.value(), datetimeFromISO8601.value(),
                        deadlineFromJSON.value());
}
} // namespace hbt::mods
