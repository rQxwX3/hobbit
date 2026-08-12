#include <recurrence/error/recurrence.hpp>
#include <recurrence/recurrence.hpp>
#include <recurrence/schema/recurrence.hpp>

namespace clndr::rec {
Recurrence::Recurrence(pattern_t pattern, dt::DateTime startDateTime,
                       dt::OptDateTime endDateTime)
    : pattern_{std::move(pattern)}, startDateTime_{startDateTime},
      endDateTime_{endDateTime} {
    schema::recurrence::Schema::validateAllRules(*this);
}

[[nodiscard]] auto Recurrence::null(dt::DateTime startDateTime) -> Recurrence {
    return {NullPattern(), startDateTime, dt::OptDateTime(std::nullopt)};
}

[[nodiscard]] auto Recurrence::getPattern() const -> pattern_t {
    return pattern_;
}

[[nodiscard]] auto Recurrence::getStartDateTime() const -> dt::DateTime {
    return startDateTime_;
}

[[nodiscard]] auto Recurrence::getEndDateTime() const -> dt::OptDateTime {
    return endDateTime_;
}

auto Recurrence::setPattern(pattern_t pattern) -> void {
    pattern_ = std::move(pattern);

    if (!fieldOK<schema::recurrence::fields::Pattern>()) {
        throw std::invalid_argument(
            std::string(error::recurrence::InvalidPattern::msg));
    }
}

auto Recurrence::setStartDateTime(dt::DateTime startDateTime) -> void {
    startDateTime_ = startDateTime;

    if (!fieldOK<schema::recurrence::fields::StartDateTime>()) {
        throw std::invalid_argument(
            std::string(error::recurrence::InvalidStart::msg));
    }
}

auto Recurrence::setEndDateTime(dt::OptDateTime endDateTime) -> void {
    endDateTime_ = endDateTime;

    if (!fieldOK<schema::recurrence::fields::EndDateTime>()) {
        throw std::invalid_argument(
            std::string(error::recurrence::InvalidEnd::msg));
    }
}

[[nodiscard]] auto Recurrence::isIntervalPattern() const -> bool {
    return getPatternType() == PatternType::Interval;
}

[[nodiscard]] auto Recurrence::isWeekdaysPattern() const -> bool {
    return getPatternType() == PatternType::Weekdays;
}

[[nodiscard]] auto Recurrence::isNullPattern() const -> bool {
    return getPatternType() == PatternType::Null;
}

[[nodiscard]] auto Recurrence::getNullPattern() const -> NullPattern {
    return std::get<NullPattern>(pattern_);
}

[[nodiscard]] auto Recurrence::getIntervalPattern() const -> IntervalPattern {
    return std::get<IntervalPattern>(pattern_);
}

[[nodiscard]] auto Recurrence::getWeekdaysPattern() const -> WeekdaysPattern {
    return std::get<WeekdaysPattern>(pattern_);
}

[[nodiscard]] auto Recurrence::happensOnDate(dt::Date date) const -> bool {
    return std::visit(
        [&](const auto &pattern) -> bool {
            return pattern.happensOnDate(date, startDateTime_);
        },
        pattern_);
}

[[nodiscard]] auto Recurrence::getDateTimesOfDate(dt::Date date) const
    -> std::vector<dt::DateTime> {
    return std::visit(
        [&](const auto &pattern) -> auto {
            return pattern.getOccurrencesOfDate(date, startDateTime_);
        },
        pattern_);
}
} // namespace clndr::rec
