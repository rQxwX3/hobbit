#include <codec/iso8601/datetime.hpp>
#include <codec/iso8601/opt_datetime.hpp>
#include <recurrence/recurrence.hpp>

namespace clndr::rec {
Recurrence::Recurrence(Validator::Validated /*unused*/, pattern_t pattern,
                       dt::DateTime startDateTime, dt::OptDateTime endDateTime)
    : pattern_{std::move(pattern)}, startDateTime_{startDateTime},
      endDateTime_{endDateTime} {}

[[nodiscard]] auto Recurrence::fromValidated(pattern_t pattern,
                                             dt::DateTime startDateTime,
                                             dt::OptDateTime endDateTime)
    -> Recurrence {
    return Recurrence(Validator::Validated{}, std::move(pattern), startDateTime,
                      endDateTime);
}

Recurrence::Recurrence(pattern_t pattern, dt::DateTime startDateTime,
                       dt::OptDateTime endDateTime)
    : pattern_{std::move(pattern)}, startDateTime_{startDateTime},
      endDateTime_{
          Validator::Return::endAfterStart(endDateTime, startDateTime)} {}

[[nodiscard]] auto Recurrence::null(dt::DateTime startDateTime) -> Recurrence {
    return {NullPattern(), startDateTime, dt::OptDateTime(std::nullopt)};
}

[[nodiscard]] auto Recurrence::getPatternType() const -> PatternType {
    if (std::holds_alternative<NullPattern>(pattern_)) {
        return PatternType::Null;
    }

    if (std::holds_alternative<IntervalPattern>(pattern_)) {
        return PatternType::Interval;
    }

    if (std::holds_alternative<WeekdaysPattern>(pattern_)) {
        return PatternType::Weekdays;
    }

    std::unreachable();
}

[[nodiscard]] auto Recurrence::getStartDateTime() const -> dt::DateTime {
    return startDateTime_;
}

[[nodiscard]] auto Recurrence::getEndDateTime() const -> dt::OptDateTime {
    return endDateTime_;
}

auto Recurrence::setPatternType(pattern_t pattern) -> void {
    pattern_ = std::move(pattern);
}

auto Recurrence::setStartDateTime(dt::DateTime startDateTime) -> void {
    startDateTime_ =
        Validator::Return::startBeforeEnd(startDateTime, getEndDateTime());
}

auto Recurrence::setEndDateTime(dt::OptDateTime endDateTime) -> void {
    endDateTime_ =
        Validator::Return::endAfterStart(endDateTime, getStartDateTime());
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

/* ------- Validator ------- */
auto Recurrence::Validator::endAfterStart(dt::OptDateTime end,
                                          dt::DateTime start) -> void {
    if (end.hasValue() && end.getValue() <= start) {
        throw std::invalid_argument(
            Error::errorMessage(Error::Code::EndBeforeStart));
    }
}

[[nodiscard]] auto Recurrence::Validator::Return::endAfterStart(
    dt::OptDateTime end, dt::DateTime start) -> dt::OptDateTime {
    Validator::endAfterStart(end, start);

    return end;
}

[[nodiscard]] auto Recurrence::Validator::Return::startBeforeEnd(
    dt::DateTime start, dt::OptDateTime end) -> dt::DateTime {
    if (end.hasValue() && start >= end.getValue()) {
        throw std::invalid_argument(
            Error::errorMessage(Error::Code::StartAfterEnd));
    }

    return start;
}
} // namespace clndr::rec
