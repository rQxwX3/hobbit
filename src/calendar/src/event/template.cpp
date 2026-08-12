#include <event/error/template.hpp>
#include <event/schema/template.hpp>
#include <event/template.hpp>

namespace clndr::ev {
Template::Template(std::string title, rec::Recurrence recurrence,
                   Deadline deadline)
    : uuid_{core::generateUUID()}, title_{std::move(title)},
      recurrence_{std::move(recurrence)}, deadline_{deadline} {
    schema::templ::Schema::validateAllRules(*this);
}

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
    title_ = title;

    schema::templ::Schema::validateAffectedRules<schema::templ::fields::Title>(
        *this);
}

auto Template::setDeadline(const Deadline &deadline) -> void {
    deadline_ = deadline;

    schema::templ::Schema::validateAffectedRules<
        schema::templ::fields::Deadline>(*this);
}

auto Template::setRecurrence(const rec::Recurrence &recurrence) -> void {
    recurrence_ = recurrence;

    schema::templ::Schema::validateAffectedRules<
        schema::templ::fields::Recurrence>(*this);
}

auto Template::setStartDateTime(const dt::DateTime &startDateTime) -> void {
    recurrence_.setStartDateTime(startDateTime);

    schema::templ::Schema::validateAffectedRules<
        schema::templ::fields::Recurrence>(*this);
}

auto Template::setEndDateTime(const dt::OptDateTime &endDateTime) -> void {
    recurrence_.setEndDateTime(endDateTime);

    schema::templ::Schema::validateAffectedRules<
        schema::templ::fields::Recurrence>(*this);
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
} // namespace clndr::ev
