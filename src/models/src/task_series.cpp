#include <task_series.hpp>

namespace hbt::mods {
auto TaskSeries::validateDeadline(Deadline deadline) -> Deadline {
    if (!deadline.isInterval()) {
        throw std::invalid_argument(errorMessage(Error::InvalidDeadline));
    }

    return deadline;
}

auto TaskSeries::validateStartDateTime(DateTime startDateTime) const
    -> DateTime {
    if (startDateTime >= endDateTime_) {
        throw std::invalid_argument(errorMessage(Error::InvalidStartDateTime));
    }

    return startDateTime;
}

auto TaskSeries::validateEndDateTime(endDateTime_t endDateTime) const
    -> endDateTime_t {
    if (endDateTime.has_value() && endDateTime.value() < getStartDateTime()) {
        throw std::invalid_argument(errorMessage(Error::InvalidEndDateTime));
    }

    return endDateTime;
}

TaskSeries::TaskSeries(TaskData taskData, util::Recurrence recurrence,
                       endDateTime_t endDateTime)
    : taskData_{std::move(taskData)}, recurrence_{std::move(recurrence)},
      endDateTime_{validateEndDateTime(endDateTime)} {}

[[nodiscard]] auto TaskSeries::getStartDateTime() const -> DateTime {
    return taskData_.getDateTime();
}

[[nodiscard]] auto TaskSeries::getEndDateTime() const -> endDateTime_t {
    return endDateTime_;
}

[[nodiscard]] auto TaskSeries::getRecurrence() const -> util::Recurrence {
    return recurrence_;
}

[[nodiscard]] auto TaskSeries::getTaskData() const -> TaskData {
    return taskData_;
}

auto TaskSeries::setTitle(std::string title) -> void {
    try {
        taskData_.setTitle(std::move(title));
    } catch (const std::exception &e) {
        rethrowTaskDataInvalidArgumentException(e);
    }
}

auto TaskSeries::setStartDateTime(DateTime startDateTime) -> void {
    startDateTime = validateStartDateTime(startDateTime);

    try {
        taskData_.setDateTime(startDateTime);
    } catch (const std::exception &e) {
        rethrowTaskDataInvalidArgumentException(e);
    }
}

auto TaskSeries::setDeadline(Deadline deadline) -> void {
    deadline = validateDeadline(deadline);

    try {
        taskData_.setDeadline(deadline);
    } catch (const std::exception &e) {
        rethrowTaskDataInvalidArgumentException(e);
    }
}

auto TaskSeries::setRecurrence(util::Recurrence recurrence) -> void {
    recurrence_ = std::move(recurrence);
}

auto TaskSeries::setEndDateTime(endDateTime_t endDateTime) -> void {
    endDateTime_ = validateEndDateTime(endDateTime);
}

[[nodiscard]] auto TaskSeries::generateSingularsForDate(DateTime datetime) const
    -> std::vector<hbt::mods::SingularTask> {
    auto results{std::vector<mods::SingularTask>{}};
    auto timestamps{recurrence_.getOccurrencesOfDate(datetime)};

    for (const auto &ts : timestamps) {
        // TODO: assert ts.getData() = datetime.getData()

        auto taskData{taskData_};
        taskData.setDateTime(ts);

        results.emplace_back(std::move(taskData));
    }

    return results;
}

[[nodiscard]] auto TaskSeries::happensOnDate(DateTime datetime) const -> bool {
    if (datetime.getDate() < getStartDateTime().getDate() ||
        (endDateTime_.has_value() &&
         datetime.getDate() > endDateTime_->getDate())) {
        return false;
    }

    return recurrence_.happensOnDate(datetime);
}

/* ------- JSON ------- */
[[nodiscard]] auto
TaskSeries::JSON::containsAllFields(const nlohmann::json &json) -> bool {
    return std::ranges::all_of(fields, [&json](const auto &field) -> bool {
        return json.contains(field);
    });
}

[[nodiscard]] auto TaskSeries::JSON::encode(const TaskSeries &taskSeries)
    -> nlohmann::json {
    auto endDateTime{taskSeries.getEndDateTime()};
    auto endDateTimeJSON{endDateTime.has_value()
                             ? endDateTime->toISO8601String()
                             : endDateTimeNullValue};

    return {
        {taskDataField, TaskData::JSON::encode(taskSeries.getTaskData())},
        {recurrenceField,
         util::Recurrence::JSON::encode(taskSeries.getRecurrence())},
        {endDateTimeField, endDateTimeJSON},
    };
}

[[nodiscard]] auto TaskSeries::JSON::decode(const nlohmann::json &json)
    -> std::expected<TaskSeries, Error> {
    if (!containsAllFields(json)) {
        return std::unexpected(JSON::Error::MissingRequiredField);
    }

    auto task{TaskData::JSON::decode(json[taskDataField])};
    if (!task) {
        return std::unexpected(JSON::Error::FailedToParseTaskData);
    }

    // TODO maybe validation here?
    auto stop{DateTime::fromISO8601String(json[endDateTimeField])};
    if (!stop) {
        return std::unexpected(JSON::Error::FailedToParseEndDateTime);
    }

    auto recurrence{util::Recurrence::JSON::decode(json[recurrenceField])};
    if (!recurrence) {
        return std::unexpected(JSON::Error::FailedToParseRecurrence);
    }

    return TaskSeries{task.value(), recurrence.value(), stop.value()};
}
} // namespace hbt::mods
