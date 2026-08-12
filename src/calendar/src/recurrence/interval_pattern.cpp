#include <datetime/interval.hpp>
#include <recurrence/error/interval_pattern.hpp>
#include <recurrence/interval_pattern.hpp>
#include <recurrence/schema/interval_pattern.hpp>

namespace clndr::rec {
IntervalPattern::IntervalPattern() = default;

IntervalPattern::IntervalPattern(dt::Interval interval) : interval_{interval} {}

[[nodiscard]] auto IntervalPattern::getType() const -> pattern::Type {
    return type;
}

[[nodiscard]] auto IntervalPattern::getInterval() const -> dt::Interval {
    return interval_;
}

[[nodiscard]] auto IntervalPattern::happensOnDate(dt::Date date,
                                                  dt::DateTime start) const
    -> bool {
    if (interval_.isZero()) {
        return dt::DateTime::equalDate(start, date);
    }

    if (interval_ < dt::Interval::days(1)) {
        return true;
    }

    // TODO: for day-based intervals use math instead of a loop

    for (auto dt{start}; dt.getDate() <= date; dt += interval_) {
        if (dt::DateTime::equalDate(dt, date)) {
            return true;
        }
    }

    return false;
}

[[nodiscard]] auto IntervalPattern::getFirstOccurrenceOfDate(
    dt::Date date, dt::DateTime start) const -> std::optional<occurrence_t> {
    for (auto dt{start}; dt.getDate() <= date; dt += interval_) {
        if (dt::DateTime::equalDate(dt, date)) {
            return dt;
        }
    }

    return std::nullopt;
}

[[nodiscard]] auto
IntervalPattern::getOccurrencesOfDate(dt::Date date, dt::DateTime start) const
    -> occurrences_t {
    auto result{occurrences_t{}};

    auto firstOccurrence{getFirstOccurrenceOfDate(date, start)};
    if (!firstOccurrence.has_value()) {
        return result;
    }

    auto endDate{(date + dt::Interval::days(1))};

    for (auto dt{firstOccurrence}; dt->getDate() != endDate; *dt += interval_) {
        result.push_back(*dt);
    }

    return result;
}

[[nodiscard]] auto
IntervalPattern::operator==(const IntervalPattern &other) const
    -> bool = default;
} // namespace clndr::rec
