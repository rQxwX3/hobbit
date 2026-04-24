#include <task_data.hpp>

namespace hbt::mods {
auto TaskData::validateDateTime(datetime_t datetime) const -> datetime_t {
    if (deadline_.has_value() &&
        (deadline_->getType() == Deadline::Type::DateTime) &&
        datetime > deadline_->getDateTime()) {
        throw std::invalid_argument(errorMessage(Error::InvalidDateTime));
    }

    return datetime;
}

auto TaskData::validateDeadline(deadline_t deadline) const -> deadline_t {
    if (!deadline || deadline->getType() != Deadline::Type::DateTime) {
        return deadline;
    }

    if (auto deadlineDT{deadline->getDateTime()}; deadlineDT < datetime_) {
        throw std::invalid_argument(errorMessage(Error::InvalidDeadline));
    }

    return deadline;
}

TaskData::TaskData(std::string title, datetime_t datetime, bool completed,
                   deadline_t deadline)
    : title_{std::move(title)}, datetime_{validateDateTime(datetime)},
      deadline_{std::move(validateDeadline(std::move(deadline)))},
      completed_{completed} {}

[[nodiscard]] auto TaskData::getTitle() const -> std::string_view {
    return title_;
}

[[nodiscard]] auto TaskData::getDateTime() const -> datetime_t {
    return datetime_;
}

[[nodiscard]] auto TaskData::getDeadline() const -> deadline_t {
    return deadline_;
}

[[nodiscard]] auto TaskData::isCompleted() const -> bool { return completed_; }

auto TaskData::setTitle(std::string title) -> void {
    title_ = std::move(title);
}

auto TaskData::setDateTime(datetime_t datetime) -> void {
    datetime_ = validateDateTime(datetime);
}

auto TaskData::setDeadline(deadline_t deadline) -> void {
    deadline_ = validateDeadline(std::move(deadline));
}

auto TaskData::setCompleted(bool completed) -> void { completed_ = completed; }

[[nodiscard]] auto TaskData::toJSON() const & -> nlohmann::json {
    nlohmann::json json = {
        {jsonTitleField, title_},
        {jsonDateTimeField, datetime_.toISO8601String()},
        {jsonDeadlineField,
         (deadline_.has_value() ? deadline_->toJSON() : jsonNullDeadlineValue)},
        {jsonCompletedField, completed_},
    };

    return json;
}

[[nodiscard]] auto TaskData::deadlineFromJSON(const nlohmann::json &json)
    -> std::expected<deadline_t, Error> {
    if (json[jsonDeadlineField] == jsonNullDeadlineValue) {
        return std::nullopt;
    }

    auto deadlineFromJSON{Deadline::fromJSON(json[jsonDeadlineField])};
    if (!deadlineFromJSON) {
        return std::unexpected(Error::JSONFailedToParseDeadline);
    }

    return deadlineFromJSON.value();
}

[[nodiscard]] auto TaskData::containsAllJSONFields(const nlohmann::json &json)
    -> bool {
    return std::ranges::all_of(jsonFields, [&json](const auto &field) -> bool {
        return json.contains(field);
    });
}

[[nodiscard]] auto TaskData::fromJSON(const nlohmann::json &json)
    -> std::expected<TaskData, Error> {
    if (!containsAllJSONFields(json)) {
        return std::unexpected(Error::JSONMissingRequiredField);
    }

    auto dateTimeFromISO8601{DateTime::fromISO8601String(
        json[jsonDateTimeField].get<std::string>())};
    if (!dateTimeFromISO8601) {
        return std::unexpected(Error::JSONFailedToParseDateTime);
    }

    auto deadline{TaskData::deadlineFromJSON(json[jsonDeadlineField])};
    if (!deadline) {
        return std::unexpected(deadline.error());
    }

    return TaskData{json[jsonTitleField].get<std::string>(),
                    dateTimeFromISO8601.value(),
                    json[jsonCompletedField].get<bool>(), deadline.value()};
}
} // namespace hbt::mods
