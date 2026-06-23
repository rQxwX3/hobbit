#include <interval_pattern.hpp>

namespace hbt::mods::util {
auto IntervalRecurrencePattern::validateInterval(const Interval &interval)
    -> Interval {
    if (interval.isZero()) {
        throw std::invalid_argument(errorMessage(Error::InvalidInterval));
    }

    return interval;
}

IntervalRecurrencePattern::IntervalRecurrencePattern(Interval interval)
    : interval_{validateInterval(interval)} {}

[[nodiscard]] auto IntervalRecurrencePattern::getInterval() const -> Interval {
    return interval_;
}

[[nodiscard]] auto
IntervalRecurrencePattern::happensOnDate(Date date, DateTime start) const
    -> bool {
    if (interval_.isZero()) {
        return DateTime::equalDate(start, date);
    }

    if (interval_ < Interval::days(1)) {
        return true;
    }

    // TODO: for day-based intervals use math instead of a loop

    for (auto dt{start}; dt.getDate() <= date; dt += interval_) {
        if (DateTime::equalDate(dt, date)) {
            return true;
        }
    }

    return false;
}

[[nodiscard]] auto IntervalRecurrencePattern::getFirstOccurrenceOfDate(
    Date date, DateTime start) const -> std::optional<occurrence_t> {
    for (auto dt{start}; dt.getDate() <= date; dt += interval_) {
        if (DateTime::equalDate(dt, date)) {
            return dt;
        }
    }

    return std::nullopt;
}

[[nodiscard]] auto
IntervalRecurrencePattern::getOccurrencesOfDate(Date date, DateTime start) const
    -> occurrences_t {
    auto result{occurrences_t{}};

    auto firstOccurrence{getFirstOccurrenceOfDate(date, start)};
    if (!firstOccurrence.has_value()) {
        return result;
    }

    auto endDate{(date + Interval::days(1)).getDate()};

    for (auto dt{firstOccurrence}; dt->getDate() != endDate; *dt += interval_) {
        result.push_back(*dt);
    }

    return result;
}

[[nodiscard]] auto IntervalRecurrencePattern::operator==(
    const IntervalRecurrencePattern &other) const -> bool = default;

/* ------- JSON ------- */
[[nodiscard]] auto
IntervalRecurrencePattern::JSON::containsAllFields(const nlohmann::json &json)
    -> bool {
    return std::ranges::all_of(fields, [json](const auto &field) -> bool {
        return json.contains(field);
    });
}

[[nodiscard]] auto IntervalRecurrencePattern::JSON::encode(
    const IntervalRecurrencePattern &pattern) -> nlohmann::json {
    return {{intervalField, pattern.getInterval().toJSON()}};
};

[[nodiscard]] auto
IntervalRecurrencePattern::JSON::decode(const nlohmann::json &json)
    -> std::expected<IntervalRecurrencePattern, Error> {
    if (!containsAllFields(json)) {
        return std::unexpected(Error::MissingRequiredField);
    }

    auto intervalFromJSON{Interval::fromJSON(json[intervalField])};
    if (!intervalFromJSON) {
        return std::unexpected(Error::FailedToParseInterval);
    }

    try {
        validateInterval(intervalFromJSON.value());
    } catch (std::invalid_argument) {
        return std::unexpected(Error::ParsedIntervalZero);
    }

    return IntervalRecurrencePattern(intervalFromJSON.value());
}
} // namespace hbt::mods::util
