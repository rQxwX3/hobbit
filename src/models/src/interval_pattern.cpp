#include <interval_pattern.hpp>

namespace hbt::mods::util {
auto IntervalRecurrencePattern::validateInterval(const Interval &interval)
    -> Interval {
    if (interval.isZero()) {
        throw std::invalid_argument(errorMessage(Error::InvalidInterval));
    }

    return interval;
}

IntervalRecurrencePattern::IntervalRecurrencePattern(DateTime start,
                                                     Interval interval)
    : start_{start}, interval_{validateInterval(interval)} {}

[[nodiscard]] auto IntervalRecurrencePattern::getInterval() const -> Interval {
    return interval_;
}

[[nodiscard]] auto IntervalRecurrencePattern::happensOnDate(DateTime on) const
    -> bool {
    if (on < start_) {
        return false;
    }

    if (interval_.isZero()) {
        return DateTime::equalDate(start_, on);
    }

    if (interval_ < Interval::days(1)) {
        return true;
    }

    // TODO: for day-based intervals use math instead of a loop

    for (auto dt{start_}; dt.getDate() <= on.getDate(); dt += interval_) {
        if (DateTime::equalDate(dt, on)) {
            return true;
        }
    }

    return false;
}

[[nodiscard]] auto
IntervalRecurrencePattern::getFirstOccurrenceOfDate(DateTime on) const
    -> std::optional<occurrence_t> {
    for (auto dt{start_}; dt.getDate() <= on.getDate(); dt += interval_) {
        if (DateTime::equalDate(dt, on)) {
            return dt;
        }
    }

    return std::nullopt;
}

[[nodiscard]] auto
IntervalRecurrencePattern::getOccurrencesOfDate(DateTime on) const
    -> occurrences_t {
    auto result{occurrences_t{}};

    auto firstOccurrence{getFirstOccurrenceOfDate(on)};
    if (!firstOccurrence.has_value()) {
        return result;
    }

    auto endDate{(on + Interval::days(1)).getDate()};

    for (auto dt{firstOccurrence}; dt->getDate() != endDate; *dt += interval_) {
        result.push_back(*dt);
    }

    return result;
}

[[nodiscard]] auto IntervalRecurrencePattern::operator==(
    const IntervalRecurrencePattern &other) const -> bool = default;

[[nodiscard]] auto
IntervalRecurrencePattern::containsAllJSONFields(const nlohmann::json &json)
    -> bool {
    return std::ranges::all_of(jsonFields, [json](const auto &field) -> bool {
        return json.contains(field);
    });
}

[[nodiscard]] auto IntervalRecurrencePattern::toJSON() const -> nlohmann::json {
    return {{jsonStartField, start_.toISO8601String()},
            {jsonIntervalField, interval_.toJSON()}};
};

[[nodiscard]] auto
IntervalRecurrencePattern::fromJSON(const nlohmann::json &json)
    -> std::expected<IntervalRecurrencePattern, Error> {
    if (!containsAllJSONFields(json)) {
        return std::unexpected(Error::JSONMissingRequiredField);
    }

    auto startFromISO8601{
        DateTime::fromISO8601String(json[jsonStartField].get<std::string>())};
    if (!startFromISO8601) {
        return std::unexpected(Error::JSONFailedToParseStart);
    }

    auto intervalFromJSON{Interval::fromJSON(json[jsonIntervalField])};
    if (!intervalFromJSON) {
        return std::unexpected(Error::JSONFailedToParseInterval);
    }

    try {
        validateInterval(intervalFromJSON.value());
    } catch (std::invalid_argument) {
        return std::unexpected(Error::JSONInvalidInterval);
    }

    return IntervalRecurrencePattern(startFromISO8601.value(),
                                     intervalFromJSON.value());
}
} // namespace hbt::mods::util
