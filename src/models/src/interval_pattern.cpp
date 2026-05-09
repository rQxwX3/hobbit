#include <interval_pattern.hpp>

namespace hbt::mods::util {
auto IntervalRecurrencePattern::validateInterval(const Interval &interval)
    -> Interval {
    if (interval.isZero()) {
        throw std::invalid_argument(errorMessage(Error::InvalidInterval));
    }

    return interval;
}

IntervalRecurrencePattern::IntervalRecurrencePattern(const Interval &interval)
    : interval_{validateInterval(interval)} {}

[[nodiscard]] auto IntervalRecurrencePattern::toJSON() const -> nlohmann::json {
    return interval_.toISO8601String();
};

[[nodiscard]] auto
IntervalRecurrencePattern::fromJSON(const nlohmann::json &json)
    -> std::expected<IntervalRecurrencePattern, Error> {
    auto intervalFromISO8601{
        Interval::fromISO8601String(json.get<std::string>())};
    if (!intervalFromISO8601) {
        return std::unexpected(Error::JSONFailedToParseInterval);
    }

    return IntervalRecurrencePattern(intervalFromISO8601.value());
}

[[nodiscard]] auto IntervalRecurrencePattern::getInterval() const -> Interval {
    return interval_;
}

[[nodiscard]] auto IntervalRecurrencePattern::happensOnDate(DateTime start,
                                                            DateTime on) const
    -> bool {
    if (interval_.isZero()) {
        return DateTime::equalDate(start, on);
    }

    if (interval_ < Interval::days(1)) {
        return true;
    }

    // TODO: for day-based intervals use math instead of a loop

    for (auto dt{start}; dt.getDate() <= on.getDate(); dt += interval_) {
        if (DateTime::equalDate(dt, on)) {
            return true;
        }
    }

    return false;
}

[[nodiscard]] auto IntervalRecurrencePattern::getFirstOccurrenceOfDate(
    DateTime start, DateTime on) const -> std::optional<occurrence_t> {
    for (auto dt{start}; dt.getDate() <= on.getDate(); dt += interval_) {
        if (DateTime::equalDate(dt, on)) {
            return dt;
        }
    }

    return std::nullopt;
}

[[nodiscard]] auto IntervalRecurrencePattern::getOccurrencesOfDate(
    DateTime start, DateTime on) const -> occurrences_t {
    auto result{occurrences_t{}};

    auto firstTS{getFirstOccurrenceOfDate(start, on)};
    if (!firstTS.has_value()) {
        return result;
    }

    auto endDate{on + Interval::days(1)};

    for (auto ts{firstTS}; ts->getDate() != endDate; *ts += interval_) {
        result.push_back(*ts);
    }

    return result;
}
} // namespace hbt::mods::util
