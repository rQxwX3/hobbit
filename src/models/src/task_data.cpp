#include <task_data.hpp>

namespace hbt::mods {
auto TaskData::validateTitle(const std::string &title) -> void {
    if (title.empty()) {
        throw std::invalid_argument(errorMessage(Error::EmptyTitle));
    }
}

auto TaskData::validateDeadlineAgainstDateTime(const Deadline &deadline,
                                               const DateTime &datetime)
    -> void {
    if (deadline.isDateTime() && deadline.getDateTime() <= datetime) {
        throw std::invalid_argument(errorMessage(Error::InvalidDeadline));
    }
}

[[nodiscard]] auto TaskData::validateAndReturnTitle(const std::string &title)
    -> std::string {
    validateTitle(title);

    return title;
}

[[nodiscard]] auto
TaskData::validateAndReturnDateTime(const DateTime &datetime) const
    -> DateTime {
    if (deadline_.isDateTime() && datetime >= deadline_.getDateTime()) {
        throw std::invalid_argument(errorMessage(Error::InvalidDateTime));
    }

    return datetime;
}

[[nodiscard]] auto
TaskData::validateAndReturnDeadline(const Deadline &deadline) const
    -> Deadline {
    validateDeadlineAgainstDateTime(deadline, datetime_);

    return deadline;
}

TaskData::TaskData(Validated, std::string title, DateTime datetime,
                   Deadline deadline)
    : title_{std::move(title)}, datetime_{datetime}, deadline_{deadline} {}

[[nodiscard]] auto TaskData::fromValidated(std::string title, DateTime datetime,
                                           Deadline deadline) -> TaskData {
    return TaskData(Validated{}, std::move(title), datetime, deadline);
}

TaskData::TaskData(std::string title, DateTime datetime, Deadline deadline)
    : title_{std::move(validateAndReturnTitle(title))}, datetime_{datetime},
      deadline_{validateAndReturnDeadline(deadline)} {}

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
    title_ = std::move(validateAndReturnTitle(title));
}

auto TaskData::setDateTime(DateTime datetime) -> void {
    datetime_ = validateAndReturnDateTime(datetime);
}

auto TaskData::setDeadline(Deadline deadline) -> void {
    deadline_ = validateAndReturnDeadline(deadline);
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

    auto titleJSON{json[titleField].get<std::string>()};
    try {
        validateTitle(titleJSON);
    } catch (std::invalid_argument) {
        return std::unexpected(JSON::Error::FailedToValidateTitle);
    }

    auto dateTimeJSON{DateTime::fromISO8601String(json[dateTimeField])};
    if (!dateTimeJSON) {
        return std::unexpected(JSON::Error::FailedToParseDateTime);
    }

    auto deadlineJSON{Deadline::fromJSON(json[deadlineField])};
    if (!deadlineJSON) {
        return std::unexpected(JSON::Error::FailedToParseDeadline);
    }

    try {
        validateDeadlineAgainstDateTime(deadlineJSON.value(),
                                        dateTimeJSON.value());
    } catch (std::invalid_argument) {
        return std::unexpected(
            JSON::Error::FailedToValidateDeadlineAgainsDateTime);
    }

    return fromValidated(titleJSON, dateTimeJSON.value(), deadlineJSON.value());
}
} // namespace hbt::mods
