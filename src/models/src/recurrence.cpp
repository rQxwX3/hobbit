#include <recurrence.hpp>

namespace hbt::mods::util {
Recurrence::Recurrence(pattern_t pattern, DateTime startDateTime,
                       OptDateTime endDateTime)
    : pattern_{std::move(pattern)}, startDateTime_{startDateTime},
      endDateTime_{
          Validator::Return::endAfterStart(endDateTime, startDateTime)} {}

[[nodiscard]] auto Recurrence::null(DateTime startDateTime) -> Recurrence {
    return {NullRecurrencePattern(), startDateTime, OptDateTime(std::nullopt)};
}

[[nodiscard]] auto Recurrence::getPatternType() const -> PatternType {
    if (std::holds_alternative<NullRecurrencePattern>(pattern_)) {
        return PatternType::Null;
    }

    if (std::holds_alternative<IntervalRecurrencePattern>(pattern_)) {
        return PatternType::Interval;
    }

    if (std::holds_alternative<WeekdaysRecurrencePattern>(pattern_)) {
        return PatternType::Weekdays;
    }

    std::unreachable();
}

[[nodiscard]] auto Recurrence::getStartDateTime() const -> DateTime {
    return startDateTime_;
}

[[nodiscard]] auto Recurrence::getEndDateTime() const -> OptDateTime {
    return endDateTime_;
}

auto Recurrence::setPatternType(pattern_t pattern) -> void {
    pattern_ = std::move(pattern);
}

auto Recurrence::setStartDateTime(DateTime startDateTime) -> void {
    startDateTime_ =
        Validator::Return::startBeforeEnd(startDateTime, getEndDateTime());
}

auto Recurrence::setEndDateTime(OptDateTime endDateTime) -> void {
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

[[nodiscard]] auto Recurrence::getIntervalPattern() const
    -> IntervalRecurrencePattern {
    return std::get<IntervalRecurrencePattern>(pattern_);
}

[[nodiscard]] auto Recurrence::getWeekdaysPattern() const
    -> WeekdaysRecurrencePattern {
    return std::get<WeekdaysRecurrencePattern>(pattern_);
}

[[nodiscard]] auto Recurrence::happensOnDate(DateTime date) const -> bool {
    return std::visit(
        [&](const auto &pattern) -> bool {
            return pattern.happensOnDate(date, startDateTime_);
        },
        pattern_);
}

[[nodiscard]] auto Recurrence::getOccurrencesOfDate(DateTime datetime) const
    -> occurrences_t {
    return std::visit(
        [&](const auto &pattern) -> auto {
            return pattern.getOccurrencesOfDate(datetime, startDateTime_);
        },
        pattern_);
}

/* ------- Validator ------- */
auto Recurrence::Validator::endAfterStart(OptDateTime end, DateTime start)
    -> void {
    if (end.hasValue() && end.getValue() <= start) {
        throw std::invalid_argument(errorMessage(Error::EndBeforeStart));
    }
}

[[nodiscard]] auto Recurrence::Validator::Return::endAfterStart(OptDateTime end,
                                                                DateTime start)
    -> OptDateTime {
    Validator::endAfterStart(end, start);

    return end;
}

[[nodiscard]] auto
Recurrence::Validator::Return::startBeforeEnd(DateTime start, OptDateTime end)
    -> DateTime {
    if (end.hasValue() && start >= end.getValue()) {
        throw std::invalid_argument(errorMessage(Error::StartAfterEnd));
    }

    return start;
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
    auto startDateTimeJSON{recurrence.getStartDateTime().toISO8601String()};
    auto endDateTimeJSON{
        OptDateTime::JSON::encode(recurrence.getEndDateTime())};

    switch (recurrence.getPatternType()) {
    case PatternType::Null:
        return {{patternTypeField, nullPatternTypeValue},
                {patternField, nullPatternTypeValue},
                {startDateTimeField, startDateTimeJSON},
                {endDateTimeField, endDateTimeJSON}};

    case PatternType::Interval:
        return {{patternTypeField, intervalPatternTypeValue},
                {patternField, IntervalRecurrencePattern::JSON::encode(
                                   recurrence.getIntervalPattern())},
                {startDateTimeField, startDateTimeJSON},
                {endDateTimeField, endDateTimeJSON}};

    case PatternType::Weekdays:
        return {{patternTypeField, weekdayPatternTypeValue},
                {patternField, WeekdaysRecurrencePattern::JSON::encode(
                                   recurrence.getWeekdaysPattern())},
                {startDateTimeField, startDateTimeJSON},
                {endDateTimeField, endDateTimeJSON}};

    default:
        throw std::runtime_error(errorMessage(Error::UnsupportedPatternType));
    }
}

[[nodiscard]] auto Recurrence::JSON::decode(const nlohmann::json &json)
    -> std::expected<Recurrence, Error> {
    if (!containsAllFields(json)) {
        return std::unexpected(JSON::Error::MissingRequiredField);
    }

    auto startDateTimeFromJSON{DateTime::fromISO8601String(
        json[startDateTimeField].get<std::string>())};
    if (!startDateTimeFromJSON) {
        return std::unexpected(Error::FailedToParseStartDateTime);
    }

    auto endDateTimeFromJSON{
        OptDateTime::JSON::decode(json[endDateTimeField].get<std::string>())};
    if (!endDateTimeFromJSON) {
        return std::unexpected(Error::FailedToParseEndDateTime);
    }

    try {
        Validator::endAfterStart(endDateTimeFromJSON.value(),
                                 startDateTimeFromJSON.value());
    } catch (std::invalid_argument) {
        return std::unexpected(Error::ParsedEndDateTimeBeforeStartDateTime);
    }

    auto patternType{json[patternTypeField].get<std::string>()};

    if (patternType == nullPatternTypeValue) {
        auto pattern{NullRecurrencePattern::JSON::decode(json[patternField])};

        if (!pattern) {
            return std::unexpected(JSON::Error::FailedToParseNullPattern);
        }

        return Recurrence(pattern.value(), startDateTimeFromJSON.value(),
                          endDateTimeFromJSON.value());
    }

    if (patternType == intervalPatternTypeValue) {
        auto pattern{
            IntervalRecurrencePattern::JSON::decode(json[patternField])};

        if (!pattern) {
            return std::unexpected(JSON::Error::FailedToParseIntervalPattern);
        }

        return Recurrence(pattern.value(), startDateTimeFromJSON.value(),
                          endDateTimeFromJSON.value());
    }

    if (patternType == weekdayPatternTypeValue) {
        auto pattern{
            WeekdaysRecurrencePattern::JSON::decode(json[patternField])};

        if (!pattern) {
            return std::unexpected(JSON::Error::FailedToParseWeekdayPattern);
        }

        return Recurrence(pattern.value(), startDateTimeFromJSON.value(),
                          endDateTimeFromJSON.value());
    }

    return std::unexpected(JSON::Error::UnsupportedPatternType);
}
} // namespace hbt::mods::util
