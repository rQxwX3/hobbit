#include <event/template.hpp>

namespace clndr::ev {
[[nodiscard]] auto Template::fromValidated(core::uuid_t uuid, std::string title,
                                           rec::Recurrence recurrence,
                                           Deadline deadline) -> Template {
    return Template(Validator::Validated{}, std::move(uuid), std::move(title),
                    std::move(recurrence), deadline);
}

Template::Template(Validator::Validated /*unused*/, core::uuid_t uuid,
                   std::string title, rec::Recurrence recurrence,
                   Deadline deadline)
    : uuid_{std::move(uuid)}, title_{std::move(title)},
      recurrence_{std::move(recurrence)}, deadline_{deadline} {}

Template::Template(std::string title, rec::Recurrence recurrence,
                   Deadline deadline)
    : uuid_{core::generateUUID()},
      title_{std::move(Validator::Return::title(title))},
      recurrence_{std::move(recurrence)},
      deadline_{Validator::Return::deadline(deadline, recurrence)} {}

[[nodiscard]] auto Template::getUUID() const -> core::uuid_t { return uuid_; }

[[nodiscard]] auto Template::getStartDateTime() const -> dt::DateTime {
    return recurrence_.getStartDateTime();
}

[[nodiscard]] auto Template::getEndDateTime() const -> dt::OptDateTime {
    return recurrence_.getEndDateTime();
}

[[nodiscard]] auto Template::getRecurrence() const -> rec::Recurrence {
    return recurrence_;
}

auto Template::setTitle(const std::string &title) -> void {
    title_ = Validator::Return::title(title);
}

auto Template::setStartDateTime(const dt::DateTime &startDateTime) -> void {
    try {
        recurrence_.setStartDateTime(startDateTime);
    } catch (const std::exception &e) {
        rethrowRecurrenceInvalidArgumentException(e);
    }
}

auto Template::setRecurrence(const rec::Recurrence &recurrence) -> void {
    recurrence_ =
        std::move(Validator::Return::recurrence(recurrence, getDeadline()));
}

auto Template::setDeadline(const Deadline &deadline) -> void {
    deadline_ = Validator::Return::deadline(deadline, getRecurrence());
}

auto Template::setEndDateTime(const dt::OptDateTime &endDateTime) -> void {
    try {
        recurrence_.setEndDateTime(endDateTime);
    } catch (const std::exception &e) {
        rethrowRecurrenceInvalidArgumentException(e);
    }
}

[[nodiscard]] auto Template::generateInstancesForDate(dt::Date date) const
    -> std::vector<ev::Instance> {
    auto result{std::vector<ev::Instance>{}};
    auto datetimes{recurrence_.getDateTimesOfDate(date)};

    const auto templateUUID{getUUID()};
    for (auto dt : datetimes) {
        result.emplace_back(templateUUID, dt, false);
    }

    return result;
}

[[nodiscard]] auto Template::happensOnDate(dt::Date date) const -> bool {
    return recurrence_.happensOnDate(date);
}

/* ------- Validator ------- */
auto Template::Validator::recurrenceCompatibleWithDeadline(
    const rec::Recurrence &recurrence, const Deadline &deadline) -> void {
    if (deadline.isDateTime() && !recurrence.isNullPattern()) {
        throw std::invalid_argument(
            errorMessage(Error::RecurrenceIncompatibleWithDeadline));
    }
}

auto Template::Validator::deadlineCompatibleWithRecurrence(
    const Deadline &deadline, const rec::Recurrence &recurrence) -> void {
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

[[nodiscard]] auto Template::Validator::Return::title(const std::string &title)
    -> std::string {
    if (title.empty()) {
        throw std::invalid_argument(errorMessage(Error::TitleEmpty));
    }

    return title;
}

[[nodiscard]] auto
Template::Validator::Return::recurrence(const rec::Recurrence &recurrence,
                                        const Deadline &deadline)
    -> rec::Recurrence {
    Validator::recurrenceCompatibleWithDeadline(recurrence, deadline);

    return recurrence;
}

auto Template::Validator::Return::deadline(const Deadline &deadline,
                                           const rec::Recurrence &recurrence)
    -> Deadline {
    Validator::deadlineCompatibleWithRecurrence(deadline, recurrence);

    return deadline;
}
} // namespace clndr::ev
