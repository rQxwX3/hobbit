#include <task_series.hpp>

namespace hbt::mods {
[[nodiscard]] auto
TaskSeries::fromValidated(std::string title, DateTime startDateTime,
                          util::Recurrence recurrence, Deadline deadline,
                          endDateTime_t endDateTime) -> TaskSeries {
    return TaskSeries(Validator::Validated{}, std::move(title), startDateTime,
                      std::move(recurrence), deadline, endDateTime);
}

TaskSeries::TaskSeries(Validator::Validated, std::string title,
                       DateTime startDateTime, util::Recurrence recurrence,
                       Deadline deadline, endDateTime_t endDateTime)
    : title_{std::move(Validator::Return::title(title))},
      startDateTime_{startDateTime}, recurrence_{std::move(recurrence)},
      deadline_{
          Validator::Return::deadline(deadline, recurrence, startDateTime)},
      endDateTime_{Validator::Return::end(endDateTime, startDateTime)} {}

TaskSeries::TaskSeries(std::string title, DateTime startDateTime,
                       util::Recurrence recurrence, Deadline deadline,
                       endDateTime_t endDateTime)
    : title_{std::move(Validator::Return::title(title))},
      startDateTime_{startDateTime}, recurrence_{std::move(recurrence)},
      deadline_{
          Validator::Return::deadline(deadline, recurrence, startDateTime)},
      endDateTime_{Validator::Return::end(endDateTime, startDateTime)} {}

[[nodiscard]] auto TaskSeries::getStartDateTime() const -> DateTime {
    return startDateTime_;
}

[[nodiscard]] auto TaskSeries::getEndDateTime() const -> endDateTime_t {
    return endDateTime_;
}

[[nodiscard]] auto TaskSeries::getRecurrence() const -> util::Recurrence {
    return recurrence_;
}

auto TaskSeries::setTitle(const std::string &title) -> void {
    title_ = Validator::Return::title(title);
}

auto TaskSeries::setStartDateTime(const DateTime &startDateTime) -> void {
    startDateTime_ = Validator::Return::startDateTime(
        startDateTime, getEndDateTime(), getDeadline());
}

auto TaskSeries::setRecurrence(const util::Recurrence &recurrence) -> void {
    recurrence_ =
        std::move(Validator::Return::recurrence(recurrence, getDeadline()));
}

auto TaskSeries::setDeadline(const Deadline &deadline) -> void {
    deadline_ = Validator::Return::deadline(deadline, getRecurrence(),
                                            getStartDateTime());
}

auto TaskSeries::setEndDateTime(const endDateTime_t &endDateTime) -> void {
    endDateTime_ = Validator::Return::end(endDateTime, getStartDateTime());
}

// [[nodiscard]] auto TaskSeries::generateSingularsForDate(DateTime datetime)
// const
//     -> std::vector<hbt::mods::SingularTask> {
//     auto results{std::vector<mods::SingularTask>{}};
//     auto timestamps{recurrence_.getOccurrencesOfDate(datetime)};
//
//     for (const auto &ts : timestamps) {
//         // TODO: assert ts.getData() = datetime.getData()
//
//         auto taskData{taskData_};
//         taskData.setDateTime(ts);
//
//         results.emplace_back(std::move(taskData));
//     }
//
//     return results;
// }

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
auto TaskSeries::Validator::deadlineCompatibleWithRecurrence(
    const Deadline &deadline, const Recurrence &recurrence) -> void {
    if (!recurrence.isNullPattern() && deadline.isDateTime()) {
        throw std::invalid_argument(
            errorMessage(Error::DeadlineIncompatibleWithRecurrence));
    }
}

auto TaskSeries::Validator::recurrenceCompatibleWithDeadline(
    const util::Recurrence &recurrence, const Deadline &deadline) -> void {
    if (deadline.isDateTime() && !recurrence.isNullPattern()) {
        throw std::invalid_argument(
            errorMessage(Error::RecurrenceIncompatibleWithDeadline));
    }
}

auto TaskSeries::Validator::deadlineAfterStartDateTime(
    const Deadline &deadline, const DateTime &startDateTime) -> void {
    if (deadline.isDateTime() && deadline.getDateTime() <= startDateTime) {
        throw std::invalid_argument(
            errorMessage(Error::DeadlineBeforeStartDateTime));
    }
}

auto TaskSeries::Validator::endAfterStart(const endDateTime_t &endDateTime,
                                          const DateTime &startDateTime)
    -> void {
    if (endDateTime.has_value() && endDateTime.value() <= startDateTime) {
        throw std::invalid_argument(
            errorMessage(Error::EndDateTimeBeforeStartDateTime));
    }
}

auto TaskSeries::Validator::startBeforeEnd(const DateTime &startDateTime,
                                           const endDateTime_t &endDateTime)
    -> void {
    if (endDateTime.has_value() && startDateTime >= endDateTime.value()) {
        throw std::invalid_argument(
            errorMessage(Error::StartDateTimeAfterEndDateTime));
    }
}

auto TaskSeries::Validator::startBeforeDeadline(const DateTime &startDateTime,
                                                const Deadline &deadline)
    -> void {
    if (deadline.isDateTime() && startDateTime >= deadline.getDateTime()) {
        throw std::invalid_argument(
            errorMessage(Error::StartDateTimeAfterDeadline));
    }
}

[[nodiscard]] auto
TaskSeries::Validator::Return::title(const std::string &title) -> std::string {
    if (title.empty()) {
        throw std::invalid_argument(errorMessage(Error::TitleEmpty));
    }

    return title;
}

[[nodiscard]] auto TaskSeries::Validator::Return::startDateTime(
    const DateTime &startDateTime, const endDateTime_t &endDateTime,
    const Deadline &deadline) -> DateTime {
    Validator::startBeforeEnd(startDateTime, endDateTime);
    Validator::startBeforeDeadline(startDateTime, deadline);

    return startDateTime;
}

[[nodiscard]] auto TaskSeries::Validator::Return::recurrence(
    const Recurrence &recurrence, const Deadline &deadline) -> Recurrence {
    Validator::recurrenceCompatibleWithDeadline(recurrence, deadline);

    return recurrence;
}

auto TaskSeries::Validator::Return::deadline(const Deadline &deadline,
                                             const Recurrence &recurrence,
                                             const DateTime &startDateTime)
    -> Deadline {
    Validator::deadlineCompatibleWithRecurrence(deadline, recurrence);
    Validator::deadlineAfterStartDateTime(deadline, startDateTime);

    return deadline;
}

auto TaskSeries::Validator::Return::end(const endDateTime_t &endDateTime,
                                        const DateTime &startDateTime)
    -> endDateTime_t {
    Validator::endAfterStart(endDateTime, startDateTime);

    return endDateTime;
}
} // namespace hbt::mods
