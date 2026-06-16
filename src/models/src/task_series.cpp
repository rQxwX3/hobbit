#include <task_series.hpp>

namespace hbt::mods {
[[nodiscard]] auto TaskSeries::fromValidated(TaskData taskData,
                                             util::Recurrence recurrence,
                                             endDateTime_t endDateTime)
    -> TaskSeries {
    return TaskSeries(Validator::Validated{}, std::move(taskData),
                      std::move(recurrence), endDateTime);
}

TaskSeries::TaskSeries(Validator::Validated, TaskData taskData,
                       util::Recurrence recurrence, endDateTime_t endDateTime)
    : taskData_{std::move(taskData)}, recurrence_{std::move(recurrence)},
      endDateTime_{endDateTime} {}

TaskSeries::TaskSeries(TaskData taskData, util::Recurrence recurrence,
                       endDateTime_t endDateTime)
    : taskData_{std::move(taskData)}, recurrence_{std::move(recurrence)},
      endDateTime_{Validator::Return::endAfterStart(endDateTime,
                                                    taskData.getDateTime())} {
    Validator::deadline(taskData.getDeadline());
}

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
    startDateTime =
        Validator::Return::startBeforeEnd(startDateTime, getEndDateTime());

    try {
        taskData_.setDateTime(startDateTime);
    } catch (const std::exception &e) {
        rethrowTaskDataInvalidArgumentException(e);
    }
}

auto TaskSeries::setDeadline(Deadline deadline) -> void {
    deadline = Validator::Return::deadline(deadline);

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
    endDateTime_ =
        Validator::Return::endAfterStart(endDateTime, getStartDateTime());
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

    auto taskData{TaskData::JSON::decode(json[taskDataField])};
    if (!taskData) {
        return std::unexpected(JSON::Error::FailedToParseTaskData);
    }

    try {
        Validator::deadline(taskData->getDeadline());
    } catch (const std::exception &e) {
        rethrowTaskDataInvalidArgumentException(e);
    }

    auto endJSON{json[endDateTimeField]};
    auto end{endDateTime_t(std::nullopt)};
    if (endJSON != endDateTimeNullValue) {
        auto decodedEnd{DateTime::fromISO8601String(endJSON)};

        if (!decodedEnd) {
            return std::unexpected(JSON::Error::FailedToParseEndDateTime);
        }

        end = decodedEnd.value();
    }

    try {
        Validator::endAfterStart(end, taskData->getDateTime());
    } catch (const std::exception &e) {
        rethrowTaskDataInvalidArgumentException(e);
    }

    auto recurrence{util::Recurrence::JSON::decode(json[recurrenceField])};
    if (!recurrence) {
        return std::unexpected(JSON::Error::FailedToParseRecurrence);
    }

    return fromValidated(taskData.value(), recurrence.value(), end);
}

/* ------- Validator ------- */
auto TaskSeries::Validator::deadline(Deadline deadline) -> void {
    if (deadline.isDateTime()) {
        throw std::invalid_argument(errorMessage(Error::InvalidDeadline));
    }
}

auto TaskSeries::Validator::endAfterStart(endDateTime_t endDateTime,
                                          DateTime startDateTime) -> void {
    if (endDateTime.has_value() && endDateTime.value() <= startDateTime) {
        throw std::invalid_argument(errorMessage(Error::InvalidEndDateTime));
    }
}

auto TaskSeries::Validator::startBeforeEnd(DateTime startDateTime,
                                           endDateTime_t endDateTime) -> void {
    if (endDateTime.has_value() && startDateTime >= endDateTime.value()) {
        throw std::invalid_argument(errorMessage(Error::InvalidStartDateTime));
    }
}

auto TaskSeries::Validator::Return::deadline(Deadline deadline) -> Deadline {
    Validator::deadline(deadline);

    return deadline;
}

auto TaskSeries::Validator::Return::endAfterStart(endDateTime_t endDateTime,
                                                  DateTime startDateTime)
    -> endDateTime_t {
    Validator::endAfterStart(endDateTime, startDateTime);

    return endDateTime;
}

auto TaskSeries::Validator::Return::startBeforeEnd(DateTime startDateTime,
                                                   endDateTime_t endDateTime)
    -> DateTime {
    Validator::startBeforeEnd(startDateTime, endDateTime);

    return startDateTime;
}

auto TaskSeries::Validator::Return::taskData(TaskData taskData,
                                             endDateTime_t endDateTime)
    -> TaskData {
    TaskSeries::Validator::startBeforeEnd(taskData.getDateTime(), endDateTime);

    return taskData;
}
} // namespace hbt::mods
