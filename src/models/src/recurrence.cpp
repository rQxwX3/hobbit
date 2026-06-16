#include <recurrence.hpp>

namespace hbt::mods::util {
Recurrence::Recurrence(pattern_t pattern) : pattern_{std::move(pattern)} {}

[[nodiscard]] auto Recurrence::getPatternType() const -> PatternType {
    if (std::holds_alternative<IntervalRecurrencePattern>(pattern_)) {
        return PatternType::Interval;
    }

    if (std::holds_alternative<WeekdaysRecurrencePattern>(pattern_)) {
        return PatternType::Weekdays;
    }

    std::unreachable();
}

[[nodiscard]] auto Recurrence::isIntervalPattern() const -> bool {
    return getPatternType() == PatternType::Interval;
}

[[nodiscard]] auto Recurrence::isWeekdaysPattern() const -> bool {
    return getPatternType() == PatternType::Weekdays;
}

[[nodiscard]] auto Recurrence::getIntervalPattern() const
    -> IntervalRecurrencePattern {
    return std::get<IntervalRecurrencePattern>(pattern_);
}

[[nodiscard]] auto Recurrence::getWeekdaysPattern() const
    -> WeekdaysRecurrencePattern {
    return std::get<WeekdaysRecurrencePattern>(pattern_);
}

[[nodiscard]] auto Recurrence::happensOnDate(DateTime on) const -> bool {
    return std::visit(
        [&](const auto &pattern) -> bool { return pattern.happensOnDate(on); },
        pattern_);
}

[[nodiscard]] auto Recurrence::getOccurrencesOfDate(DateTime datetime) const
    -> occurrences_t {
    return std::visit(
        [&](const auto &pattern) -> auto {
            return pattern.getOccurrencesOfDate(datetime);
        },
        pattern_);
}

/* ------- JSON ------- */
[[nodiscard]] auto
Recurrence::JSON::containsAllFields(const nlohmann::json &json) -> bool {
    return std::ranges::all_of(fields, [&json](const auto &field) -> bool {
        return json.contains(field);
    });
}

[[nodiscard]] auto Recurrence::JSON::encode(const Recurrence &recurrence)
    -> nlohmann::json {
    switch (recurrence.getPatternType()) {
    case PatternType::Interval:
        return {{patternTypeField, intervalPatternTypeValue},
                {patternField, recurrence.getIntervalPattern().toJSON()}};

    case PatternType::Weekdays:
        return {{patternTypeField, weekdayPatternTypeValue},
                {patternField, WeekdaysRecurrencePattern::JSON::encode(
                                   recurrence.getWeekdaysPattern())}};

    default:
        throw std::runtime_error(errorMessage(Error::UnsupportedPatternType));
    }
}

[[nodiscard]] auto Recurrence::JSON::decode(const nlohmann::json &json)
    -> std::expected<Recurrence, Error> {
    if (!containsAllFields(json)) {
        return std::unexpected(JSON::Error::MissingRequiredField);
    }

    auto patternType{json[patternTypeField].get<std::string>()};

    if (patternType == intervalPatternTypeValue) {
        auto pattern{IntervalRecurrencePattern::fromJSON(json[patternField])};

        if (!pattern) {
            return std::unexpected(JSON::Error::FailedToParseIntervalPattern);
        }

        return Recurrence(pattern.value());
    }

    if (patternType == weekdayPatternTypeValue) {
        auto pattern{
            WeekdaysRecurrencePattern::JSON::decode(json[patternField])};

        if (!pattern) {
            return std::unexpected(JSON::Error::FailedToParseWeekdayPattern);
        }

        return Recurrence(pattern.value());
    }

    return std::unexpected(JSON::Error::UnsupportedPatternType);
}
} // namespace hbt::mods::util
