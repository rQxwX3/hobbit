#include <event/error/template.hpp>
#include <event/schema/template.hpp>
#include <event/template.hpp>

namespace clndr::ev {
Template::Template(std::string title, rec::Recurrence recurrence,
                   Deadline deadline)
    : uuid_{core::generateUUID()}, title_{std::move(title)},
      recurrence_{std::move(recurrence)}, deadline_{deadline} {}

[[nodiscard]] auto Template::ok() const -> bool {
    return schema::templ::Schema::validate(*this);
}

template <typename Field> [[nodiscard]] auto Template::fieldOK() const -> bool {
    return schema::templ::Schema::validateAffectedRules<Field>(*this);
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

    if (!fieldOK<schema::templ::fields::Title>()) {
        throw std::invalid_argument(
            std::string(error::templ::InvalidTitle::msg));
    }
}

auto Template::setDeadline(const Deadline &deadline) -> void {
    deadline_ = deadline;

    if (!fieldOK<schema::templ::fields::Deadline>()) {
        throw std::invalid_argument(
            std::string(error::templ::InvalidDeadline::msg));
    }
}

auto Template::setRecurrence(const rec::Recurrence &recurrence) -> void {
    recurrence_ = recurrence;

    if (!fieldOK<schema::templ::fields::Recurrence>()) {
        throw std::invalid_argument(
            std::string(error::templ::InvalidRecurrence::msg));
    }
}

auto Template::setStartDateTime(const dt::DateTime &startDateTime) -> void {
    recurrence_.setStartDateTime(startDateTime);

    if (!fieldOK<schema::templ::fields::Recurrence>()) {
        throw std::invalid_argument(
            std::string(error::templ::InvalidStartDateTime::msg));
    }
}

auto Template::setEndDateTime(const dt::OptDateTime &endDateTime) -> void {
    recurrence_.setEndDateTime(endDateTime);

    if (!fieldOK<schema::templ::fields::Recurrence>()) {
        throw std::invalid_argument(
            std::string(error::templ::InvalidEndDateTime::msg));
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
} // namespace clndr::ev
