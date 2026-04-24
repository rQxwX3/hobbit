#include <recurrence.hpp>

namespace hbt::mods::util {
Recurrence::Recurrence(pattern_t pattern) : pattern_{std::move(pattern)} {}

[[nodiscard]] auto Recurrence::getPatternType() const -> PatternType {
    if (std::holds_alternative<IntervalRecurrencePattern>(pattern_)) {
        return PatternType::Interval;
    }

    if (std::holds_alternative<WeekdayRecurrencePattern>(pattern_)) {
        return PatternType::Weekday;
    }

    throw std::runtime_error(errorMessage(Error::UnsupportedPatternType));
}

[[nodiscard]] auto Recurrence::getIntervalPattern() const
    -> IntervalRecurrencePattern {
    return std::get<IntervalRecurrencePattern>(pattern_);
}

[[nodiscard]] auto Recurrence::getWeekdayPattern() const
    -> WeekdayRecurrencePattern {
    return std::get<WeekdayRecurrencePattern>(pattern_);
}

[[nodiscard]] auto Recurrence::getOccurrencesOnDate(DateTime start,
                                                    Date date) const
    -> occurrences_t {
    return std::visit(
        [&](const auto &pattern) -> auto {
            return pattern.getOccurrencesOnDate(start, date);
        },
        pattern_);
}

[[nodiscard]] auto Recurrence::isForDate(DateTime start, Date date) const
    -> bool {
    return std::visit(
        [&](const auto &pattern) -> bool {
            return pattern.happensOnDate(start, date);
        },
        pattern_);
}

[[nodiscard]] auto Recurrence::toJSON() const -> nlohmann::json {
    switch (getPatternType()) {
    case PatternType::Interval:
        return {{jsonPatternTypeField, jsonIntervalPatternTypeValue},
                {jsonPatternField, getIntervalPattern().toJSON()}};

    case PatternType::Weekday:
        return {{jsonPatternTypeField, jsonWeekdayPatternTypeValue},
                {jsonPatternField, getWeekdayPattern().toJSON()}};

    default:
        throw std::runtime_error(errorMessage(Error::UnsupportedPatternType));
    }
}

[[nodiscard]] auto Recurrence::containsAllJSONFields(const nlohmann::json &json)
    -> bool {
    return std::ranges::all_of(jsonFields, [&json](const auto &field) -> bool {
        return json.contains(field);
    });
}

[[nodiscard]] auto Recurrence::fromJSON(const nlohmann::json &json)
    -> std::expected<Recurrence, Error> {
    if (!containsAllJSONFields(json)) {
        return std::unexpected(Error::JSONMissingRequiredField);
    }

    auto patternType{json[jsonPatternTypeField]};

    if (patternType == jsonIntervalPatternTypeValue) {
        auto pattern{
            IntervalRecurrencePattern::fromJSON(json[jsonPatternField])};

        if (!pattern) {
            return std::unexpected(Error::JSONFailedToParseIntervalPattern);
        }

        return Recurrence(pattern.value());
    }

    if (patternType == jsonWeekdayPatternTypeValue) {
        auto pattern{
            WeekdayRecurrencePattern::fromJSON(json[jsonPatternField])};

        if (!pattern) {
            return std::unexpected(Error::JSONFailedToParseWeekdayPattern);
        }

        return Recurrence(pattern.value());
    }

    return std::unexpected(Error::JSONUnsupportedPatternType);
}
} // namespace hbt::mods::util
