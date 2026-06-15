#include <task_data.hpp>

namespace hbt::mods {
[[nodiscard]] auto TaskData::validateTitle(const std::string &title)
    -> std::string {
    if (title.empty()) {
        throw std::invalid_argument(errorMessage(Error::EmptyTitle));
    }

    return title;
}

[[nodiscard]] auto TaskData::validateDateTime(const DateTime &datetime) const
    -> DateTime {
    if (deadline_.isDateTime() && datetime > deadline_.getDateTime()) {
        throw std::invalid_argument(errorMessage(Error::InvalidDateTime));
    }

    return datetime;
}

[[nodiscard]] auto TaskData::validateDeadline(const Deadline &deadline) const
    -> Deadline {
    if (deadline.isDateTime() && deadline.getDateTime() < datetime_) {
        throw std::invalid_argument(errorMessage(Error::InvalidDeadline));
    }

    return deadline;
}

TaskData::TaskData(std::string title, DateTime datetime, Deadline deadline)
    : title_{std::move(validateTitle(title))}, datetime_{datetime},
      deadline_{validateDeadline(deadline)} {}

[[nodiscard]] auto TaskData::getTitle() const -> std::string_view {
    return title_;
}

[[nodiscard]] auto TaskData::getDateTime() const -> DateTime {
    return datetime_;
}

[[nodiscard]] auto TaskData::getDeadline() const -> Deadline {
    return deadline_;
}

auto TaskData::setTitle(std::string title) -> void {
    title_ = std::move(validateTitle(title));
}

auto TaskData::setDateTime(DateTime datetime) -> void {
    datetime_ = validateDateTime(datetime);
}

auto TaskData::setDeadline(Deadline deadline) -> void {
    deadline_ = validateDeadline(deadline);
}

/* ------- JSON ------- */
[[nodiscard]] auto TaskData::JSON::containsAllFields(const nlohmann::json &json)
    -> bool {
    return std::ranges::all_of(fields, [json](const auto &field) -> bool {
        return json.contains(field);
    });
}

[[nodiscard]] auto TaskData::JSON::encode(const TaskData &taskData)
    -> nlohmann::json {
    nlohmann::json json = {
        {titleField, taskData.getTitle()},
        {dateTimeField, taskData.getDateTime().toISO8601String()},
        {dateTimeField, taskData.getDeadline().toJSON()}};

    return json;
}

[[nodiscard]] auto TaskData::JSON::decode(const nlohmann::json &json)
    -> std::expected<TaskData, JSON::Error> {
    if (!containsAllFields(json)) {
        return std::unexpected(Error::MissingRequiredField);
    }

    auto dateTimeFromJSON{DateTime::fromISO8601String(json[dateTimeField])};
    if (!dateTimeFromJSON) {
        return std::unexpected(JSON::Error::FailedToParseDateTime);
    }

    auto deadlineFromJSON{Deadline::fromJSON(json[deadlineField])};
    if (!deadlineFromJSON) {
        return std::unexpected(JSON::Error::FailedToParseDeadline);
    }

    return TaskData{json[titleField].get<std::string>(),
                    dateTimeFromJSON.value(), deadlineFromJSON.value()};
}
} // namespace hbt::mods
