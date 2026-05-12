#include <weekdays_pattern.hpp>

namespace hbt::mods::util {
auto WeekdaysRecurrencePattern::validateInterval(const Interval &interval)
    -> Interval {
    if (!interval.onlyContainsUnit(Interval::Unit::WEEK)) {
        throw std::invalid_argument(errorMessage(Error::InvalidInterval));
    }

    return interval;
}

auto WeekdaysRecurrencePattern::validateFirstWeek(const firstWeek_t &firstWeek)
    -> firstWeek_t {
    if (std::ranges::none_of(firstWeek.begin(), firstWeek.end(),
                             [](auto el) -> bool { return el.has_value(); })) {
        throw std::invalid_argument(errorMessage(Error::InvalidFirstWeek));
    }

    return firstWeek;
}
WeekdaysRecurrencePattern::WeekdaysRecurrencePattern(DateTime start,
                                                     Weekdays weekdays,
                                                     Interval interval)
    : firstWeek_{validateFirstWeek(createFirstWeek(start, weekdays))},
      interval_{validateInterval(interval)} {}

[[nodiscard]] auto WeekdaysRecurrencePattern::getInterval() const -> Interval {
    return interval_;
}

[[nodiscard]] auto WeekdaysRecurrencePattern::happensOnDate(DateTime on) const
    -> bool {
    auto onWeekday{on.getWeekday()};

    auto firstInstanceOfWeekday(firstWeek_[static_cast<size_t>(onWeekday)]);
    if (!firstInstanceOfWeekday.has_value()) {
        return false;
    }

    return DateTime::daysDiff(firstInstanceOfWeekday.value(), on)
        .isMultipleOf(interval_);
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

    for (auto i{0}; i != static_cast<size_t>(Weekdays::Week::COUNT_); ++i) {
        auto dt{firstWeek_[i]};

        if (!dt.has_value()) {
            firstWeekJSON[i] = jsonFirstWeekNullValue;
        }

        firstWeekJSON[i] = dt->toISO8601String();
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

    if (json.size() != static_cast<size_t>(Weekdays::Week::COUNT_)) {
        return std::unexpected(Error::JSONFirstWeekInvalidCount);
    }

    for (auto i{0}; i != static_cast<size_t>(Weekdays::Week::COUNT_); ++i) {
        auto jsonDT{json[i]};

        if (!jsonDT.is_string()) {
            return std::unexpected(Error::JSONFirstWeekArrayIsNotOfStrings);
        }

        auto stringDT{jsonDT.get<std::string>()};
        if (stringDT == jsonFirstWeekNullValue) {
            result[i] = std::nullopt;
        }

        auto dtFromISO8601{DateTime::fromISO8601String(stringDT)};
        if (!dtFromISO8601) {
            return std::unexpected(Error::JSONFirstWeekFailedToParseDateTime);
        }

        result[i] = dtFromISO8601.value();
    }
}

[[nodiscard]] auto WeekdaysRecurrencePattern::toJSON() const -> nlohmann::json {
    return {{jsonFirstWeekField, firstWeekToJSON()},
            {jsonIntervalField, interval_.toISO8601String()}};
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

    auto firstWeekFromJSON{
        WeekdaysRecurrencePattern::firstWeekFromJSON(json[jsonFirstWeekField])};
    if (!firstWeekFromJSON) {
        return std::unexpected(Error::JSONFailedToParseFirstWeek);
    }

    try {
        validateFirstWeek(firstWeekFromJSON.value());
    } catch (std::invalid_argument) {
        return std::unexpected(Error::JSONInvalidFirstWeek);
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

    return WeekdaysRecurrencePattern(firstWeekFromJSON.value(),
                                     intervalFromJSON.value());
}
} // namespace hbt::mods::util
