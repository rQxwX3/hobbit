#include <task_series.hpp>

namespace hbt::mods {
[[nodiscard]] auto TaskSeries::fromValidated(uuid_t uuid, std::string title,
                                             util::Recurrence recurrence,
                                             Deadline deadline) -> TaskSeries {
    return TaskSeries(Validator::Validated{}, std::move(uuid), std::move(title),
                      std::move(recurrence), deadline);
}

TaskSeries::TaskSeries(Validator::Validated, uuid_t uuid, std::string title,
                       util::Recurrence recurrence, Deadline deadline)
    : uuid_{std::move(uuid)}, title_{std::move(title)},
      recurrence_{std::move(recurrence)}, deadline_{deadline} {}

TaskSeries::TaskSeries(std::string title, util::Recurrence recurrence,
                       Deadline deadline)
    : uuid_{core::uuid::generateUUID()},
      title_{std::move(Validator::Return::title(title))},
      recurrence_{std::move(recurrence)},
      deadline_{Validator::Return::deadline(deadline, recurrence)} {}

[[nodiscard]] auto TaskSeries::getUUID() const -> uuid_t { return uuid_; }

[[nodiscard]] auto TaskSeries::getStartDateTime() const -> DateTime {
    return recurrence_.getStartDateTime();
}

[[nodiscard]] auto TaskSeries::getEndDateTime() const -> OptDateTime {
    return recurrence_.getEndDateTime();
}

[[nodiscard]] auto TaskSeries::getRecurrence() const -> util::Recurrence {
    return recurrence_;
}

auto TaskSeries::setTitle(const std::string &title) -> void {
    title_ = Validator::Return::title(title);
}

auto TaskSeries::setStartDateTime(const DateTime &startDateTime) -> void {
    try {
        recurrence_.setStartDateTime(startDateTime);
    } catch (const std::exception &e) {
        rethrowRecurrenceInvalidArgumentException(e);
    }
}

auto TaskSeries::setRecurrence(const util::Recurrence &recurrence) -> void {
    recurrence_ =
        std::move(Validator::Return::recurrence(recurrence, getDeadline()));
}

auto TaskSeries::setDeadline(const Deadline &deadline) -> void {
    deadline_ = Validator::Return::deadline(deadline, getRecurrence());
}

auto TaskSeries::setEndDateTime(const OptDateTime &endDateTime) -> void {
    try {
        recurrence_.setEndDateTime(endDateTime);
    } catch (const std::exception &e) {
        rethrowRecurrenceInvalidArgumentException(e);
    }
}

[[nodiscard]] auto TaskSeries::generateInstancesForDate(Date date) const
    -> std::vector<task::Instance> {
    auto result{std::vector<task::Instance>{}};
    auto datetimes{recurrence_.getDateTimesOfDate(date)};

    for (auto dt : datetimes) {
        result.emplace_back(getUUID(), dt, false);
    }

    return result;
}

[[nodiscard]] auto TaskSeries::happensOnDate(Date date) const -> bool {
    return recurrence_.happensOnDate(date);
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
    return {
        {uuidField, taskSeries.uuid_},
        {titleField, taskSeries.getTitle()},
        {recurrenceField, Recurrence::JSON::encode(taskSeries.getRecurrence())},
        {deadlineField, taskSeries.getDeadline().toJSON()}};
}

[[nodiscard]] auto TaskSeries::JSON::decode(const nlohmann::json &json)
    -> std::expected<TaskSeries, JSON::Error> {
    if (!containsAllFields(json)) {
        return std::unexpected(Error::MissingRequiredField);
    }

    auto uuid{json[uuidField].get<std::string>()};

    auto title{json[titleField].get<std::string>()};
    try {
        Validator::title(title);
    } catch (const std::exception &e) {
        return std::unexpected(Error::FailedToValidateTitle);
    }

    auto recurrence{Recurrence::JSON::decode(json[recurrenceField])};
    if (!recurrence) {
        return std::unexpected(Error::FailedToParseRecurrence);
    }

    auto deadline(Deadline::fromJSON(json[deadlineField]));
    if (!deadline) {
        return std::unexpected(Error::FailedToParseDeadline);
    }

    try {
        Validator::deadlineCompatibleWithRecurrence(deadline.value(),
                                                    recurrence.value());
    } catch (const std::exception &e) {
        return std::unexpected(Error::FailedToValidateDeadline);
    }

    return fromValidated(uuid, title, recurrence.value(), deadline.value());
}

/* ------- Validator ------- */
auto TaskSeries::Validator::recurrenceCompatibleWithDeadline(
    const util::Recurrence &recurrence, const Deadline &deadline) -> void {
    if (deadline.isDateTime() && !recurrence.isNullPattern()) {
        throw std::invalid_argument(
            errorMessage(Error::RecurrenceIncompatibleWithDeadline));
    }
}

auto TaskSeries::Validator::deadlineCompatibleWithRecurrence(
    const Deadline &deadline, const Recurrence &recurrence) -> void {
    if (deadline.isDateTime() && !recurrence.isNullPattern()) {
        throw std::invalid_argument(
            errorMessage(Error::DeadlineIncompatibleWithRecurrence));
    }

    if (deadline.isDateTime() &&
        recurrence.getStartDateTime() > deadline.getDateTime()) {
        throw std::invalid_argument(
            errorMessage(Error::DeadlineBeforeStartDateTime));
    }
}

[[nodiscard]] auto
TaskSeries::Validator::Return::title(const std::string &title) -> std::string {
    if (title.empty()) {
        throw std::invalid_argument(errorMessage(Error::TitleEmpty));
    }

    return title;
}

[[nodiscard]] auto TaskSeries::Validator::Return::recurrence(
    const Recurrence &recurrence, const Deadline &deadline) -> Recurrence {
    Validator::recurrenceCompatibleWithDeadline(recurrence, deadline);

    return recurrence;
}

auto TaskSeries::Validator::Return::deadline(const Deadline &deadline,
                                             const Recurrence &recurrence)
    -> Deadline {
    Validator::deadlineCompatibleWithRecurrence(deadline, recurrence);

    return deadline;
}
} // namespace hbt::mods
