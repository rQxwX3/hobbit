#include <task_series.hpp>

namespace hbt::mods {
auto TaskSeries::validateDeadline(deadline_t deadline) const -> deadline_t {
    if (deadline.has_value() &&
        deadline->getType() != Deadline::Type::Interval) {
        throw std::invalid_argument(errorMessage(Error::InvalidDeadline));
    }

    return deadline;
}

auto TaskSeries::validateStart(start_t start) const -> start_t {
    if (stop_.has_value() && start > stop_.value()) {
        throw std::invalid_argument(errorMessage(Error::InvalidStart));
    }

    return start;
}

auto TaskSeries::validateStop(stop_t stop) const -> stop_t {
    if (stop.has_value() && stop.value() < getStart()) {
        throw std::invalid_argument(errorMessage(Error::InvalidStop));
    }

    return stop;
}

TaskSeries::TaskSeries(TaskData task, util::Recurrence recurrence, stop_t stop)
    : task_{std::move(task)}, recurrence_{std::move(recurrence)},
      stop_{validateStop(stop)} {
    validateDeadline(task_.getDeadline());
}

// [[nodiscard]] auto
// TaskSeries::generateFirstSingularOfDate(mods::DateTime datetime)
//     -> std::optional<hbt::mods::SingularTask> {
//     if (std::holds_alternative<mods::util::WeekdayRecurrence>(
//             recurrencePattern_)) {
//     }
//
//     for (auto dt{task_.start}; !mods::DateTime::equalDates(dt, datetime);)
// }
//
//
[[nodiscard]] auto TaskSeries::getStart() const -> start_t {
    return task_.getDateTime();
}

[[nodiscard]] auto TaskSeries::getStop() const -> stop_t { return stop_; }

[[nodiscard]] auto TaskSeries::getRecurrence() const -> util::Recurrence {
    return recurrence_;
}

auto TaskSeries::setStart(start_t start) -> void {
    task_.setDateTime(validateStart(start));
}

auto TaskSeries::setStop(stop_t stop) -> void { stop_ = validateStop(stop); }

auto TaskSeries::setDeadline(deadline_t deadline) -> void {
    task_.setDeadline(validateDeadline(std::move(deadline)));
}

auto TaskSeries::setRecurrence(util::Recurrence recurrence) -> void {
    recurrence_ = std::move(recurrence);
}

[[nodiscard]] auto TaskSeries::generateSingularsForDate(mods::Date date) const
    -> std::vector<hbt::mods::SingularTask> {
    auto results{std::vector<mods::SingularTask>{}};
    auto occurrences{recurrence_.getOccurrencesOnDate(getStart(), date)};

    for (const auto &dt : occurrences) {
        // TODO: assert dt.getData() = datetime.getData()

        auto taskData{task_};
        taskData.setDateTime(dt);

        results.emplace_back(std::move(taskData));
    }

    return results;
}

[[nodiscard]] auto TaskSeries::isForDate(Date date) const -> bool {
    if (date < getStart().getDate() ||
        (stop_.has_value() && date > stop_->getDate())) {
        return false;
    }

    return recurrence_.isForDate(getStart(), date);
}

[[nodiscard]] auto TaskSeries::containsAllJSONFields(const nlohmann::json &json)
    -> bool {
    return std::ranges::all_of(jsonFields, [&json](const auto &field) -> bool {
        return json.contains(field);
    });
}

[[nodiscard]] auto TaskSeries::toJSON() const -> nlohmann::json {
    return {{jsonTaskField, task_.toJSON()},
            {jsonStopField, stop_->toISO8601String()},
            {jsonRecurrenceField, recurrence_.toJSON()}};
}

[[nodiscard]] auto TaskSeries::fromJSON(const nlohmann::json &json)
    -> std::expected<TaskSeries, Error> {
    if (!containsAllJSONFields(json)) {
        return std::unexpected(Error::JSONMissingRequiredField);
    }

    auto task{TaskData::fromJSON(json[jsonTaskField])};
    if (!task) {
        return std::unexpected(Error::JSONFailedToParseTaskData);
    }

    // maybe validation here?
    auto stop{DateTime::fromISO8601String(json[jsonStopField])};
    if (!stop) {
        return std::unexpected(Error::JSONFailedToParseStop);
    }

    auto recurrence{util::Recurrence::fromJSON(json[jsonRecurrenceField])};
    if (!recurrence) {
        return std::unexpected(Error::JSONFailedToParseRecurrence);
    }

    return TaskSeries{task.value(), recurrence.value(), stop.value()};
}
} // namespace hbt::mods
