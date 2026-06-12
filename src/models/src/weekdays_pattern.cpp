#include <weekdays_pattern.hpp>

namespace hbt::mods::util {
auto WeekdaysRecurrencePattern::validateInterval(const Interval &interval)
    -> Interval {
    if (!interval.onlyContainsUnit(Interval::Unit::WEEK)) {
        throw std::invalid_argument(errorMessage(Error::InvalidInterval));
    }

    return interval;
}

auto WeekdaysRecurrencePattern::validateWeek(const Week &week) -> Week {
    if (week.isEmpty()) {
        throw std::invalid_argument(errorMessage(Error::EmptyWeek));
    }

    return week;
}

WeekdaysRecurrencePattern::WeekdaysRecurrencePattern(DateTime start, Week week,
                                                     Interval interval)
    : firstWeek_{createFirstWeek(start, validateWeek(week))},
      interval_{validateInterval(interval)} {}

WeekdaysRecurrencePattern::WeekdaysRecurrencePattern(firstWeek_t firstWeek,
                                                     Interval interval)
    : firstWeek_{firstWeek}, interval_{validateInterval(interval)} {}

[[nodiscard]] auto WeekdaysRecurrencePattern::getInterval() const -> Interval {
    return interval_;
}

[[nodiscard]] auto WeekdaysRecurrencePattern::happensOnDate(DateTime on) const
    -> bool {
    auto firstInstanceOfWeekday(
        firstWeek_[static_cast<size_t>(on.getWeekday())]);

    if (!firstInstanceOfWeekday.has_value()) {
        return false;
    }

    auto daysDiff{DateTime::daysDiff(firstInstanceOfWeekday.value(), on)};

    return daysDiff.isZero() ||
           (on > firstInstanceOfWeekday && daysDiff.isMultipleOf(interval_));
}

[[nodiscard]] auto
WeekdaysRecurrencePattern::getOccurrencesOfDate(DateTime on) const
    -> occurrences_t {
    if (happensOnDate(on)) {
        return {DateTime(on)};
    }

    return {};
}

[[nodiscard]] auto WeekdaysRecurrencePattern::firstWeekToJSON() const
    -> nlohmann::json {
    auto firstWeekJSON = nlohmann::json{{}};

    for (auto i{0}; i != Week::weekdaysCount; ++i) {
        auto dt{firstWeek_[i]};

        firstWeekJSON[i] =
            dt.has_value() ? dt->toISO8601String() : jsonFirstWeekNullValue;
    }

    return firstWeekJSON;
}

[[nodiscard]] auto
WeekdaysRecurrencePattern::firstWeekFromJSON(const nlohmann::json &json)
    -> std::expected<firstWeek_t, Error> {
    auto result{firstWeek_t()};

    if (!json.is_array()) {
        return std::unexpected(Error::JSONFirstWeekNotArray);
    }

    if (json.size() != Week::weekdaysCount) {
        return std::unexpected(Error::JSONFirstWeekInvalidCount);
    }

    for (auto i{0}; i != Week::weekdaysCount; ++i) {
        auto jsonDT = json[i];

        if (!jsonDT.is_string()) {
            return std::unexpected(Error::JSONFirstWeekArrayIsNotOfStrings);
        }

        auto dtString{jsonDT.get<std::string>()};
        if (dtString == jsonFirstWeekNullValue) {
            result[i] = std::nullopt;
            continue;
        }

        auto dtFromISO8601{DateTime::fromISO8601String(dtString)};
        if (!dtFromISO8601) {
            return std::unexpected(Error::JSONFirstWeekFailedToParseDateTime);
        }

        result[i] = dtFromISO8601.value();
    }

    return result;
}

[[nodiscard]] auto WeekdaysRecurrencePattern::toJSON() const -> nlohmann::json {
    return {{jsonFirstWeekField, firstWeekToJSON()},
            {jsonIntervalField, interval_.toJSON()}};
}

[[nodiscard]] auto
WeekdaysRecurrencePattern::containsAllJSONFields(const nlohmann::json &json)
    -> bool {
    return std::ranges::all_of(jsonFields, [&json](const auto &field) -> bool {
        return json.contains(field);
    });
}

[[nodiscard]] auto
WeekdaysRecurrencePattern::fromJSON(const nlohmann::json &json)
    -> std::expected<WeekdaysRecurrencePattern, Error> {
    if (!containsAllJSONFields(json)) {
        return std::unexpected(Error::JSONMissingRequiredField);
    }

    auto firstWeekFromJSON =
        WeekdaysRecurrencePattern::firstWeekFromJSON(json[jsonFirstWeekField]);
    if (!firstWeekFromJSON) {
        return std::unexpected(Error::JSONFailedToParseFirstWeek);
    }

    auto intervalFromJSON = Interval::fromJSON(json[jsonIntervalField]);
    if (!intervalFromJSON) {
        return std::unexpected(Error::JSONFailedToParseInterval);
    }

    try {
        validateInterval(intervalFromJSON.value());
    } catch (std::invalid_argument) {
        return std::unexpected(Error::JSONInvalidInterval);
    }

    return WeekdaysRecurrencePattern(firstWeekFromJSON.value(),
                                     intervalFromJSON.value());
}
} // namespace hbt::mods::util
