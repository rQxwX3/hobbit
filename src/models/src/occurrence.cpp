#include <occurrence.hpp>

namespace hbt::mods {
[[nodiscard]] auto Occurrence::NonRecurrent::toJSON() const -> nlohmann::json {
    return {{"type", "non-recurrent"}};
}

[[nodiscard]] auto
Occurrence::NonRecurrent::fromJSON(const nlohmann::json &json)
    -> std::optional<NonRecurrent> {
    return NonRecurrent{};
}

[[nodiscard]] auto Occurrence::IntervalRecurrent::toJSON() const
    -> nlohmann::json {
    return {{"type", "interval-recurrent"}, {"interval", interval.toJSON()}};
}

[[nodiscard]] auto
Occurrence::IntervalRecurrent::fromJSON(const nlohmann::json &json)
    -> std::optional<IntervalRecurrent> {
    if (!json.contains("interval")) {
        return std::nullopt;
    }

    auto intervalFromJSON{hbt::mods::Interval::fromJSON(json["interval"])};
    if (!intervalFromJSON.has_value()) {
        return std::nullopt;
    }

    return IntervalRecurrent{intervalFromJSON.value()};
}

[[nodiscard]] auto Occurrence::WeekdayRecurrent::toJSON() const
    -> nlohmann::json {
    return {
        {"type", "weekday-recurrent"},
        {"weekdays", week.getDays().to_string()},
        {"interval", interval.toJSON()},
    };
}

[[nodiscard]] auto
Occurrence::WeekdayRecurrent::fromJSON(const nlohmann::json &json)
    -> std::optional<WeekdayRecurrent> {
    if (!json.contains("interval") || !json.contains("weekdays")) {
        return std::nullopt;
    }

    auto intervalFromJSON{hbt::mods::Interval::fromJSON(json["interval"])};

    if (!intervalFromJSON.has_value()) {
        return std::nullopt;
    }

    return WeekdayRecurrent{.week = Week{json["weekdays"].get<std::string>()},
                            .interval = intervalFromJSON.value()};
}

Occurrence::Occurrence(hbt::mods::DateTime datetime,
                       recurrencePattern_t recurrenceModel)
    : datetime_{datetime}, recurrenceModel_{std::move(recurrenceModel)} {}

Occurrence::Occurrence(hbt::mods::DateTime datetime,
                       const hbt::mods::Interval &interval)
    : datetime_{datetime}, recurrenceModel_{IntervalRecurrent{interval}} {}

Occurrence::Occurrence(hbt::mods::DateTime datetime, hbt::mods::Week week,
                       const hbt::mods::Interval &interval)
    : datetime_{datetime} {
    if (!interval.onlyContainsUnit(DurationUnits::WEEK)) {
        throw std::invalid_argument(
            "Interval for weekday-recurrent occurrences must be in weeks");
    }

    if (week.getDays().none()) {
        throw std::invalid_argument("At least one weekday must be provided for "
                                    "weekday-reccurrent occurrences");
    }

    if (!week.getDays().test(static_cast<size_t>(datetime.getWeekday()))) {
        throw std::invalid_argument(
            "Weekday of weekday-recurrent event's start date must be listed in "
            "weekdays list argument");
    }

    recurrenceModel_ = WeekdayRecurrent{.week = week, .interval = interval};
}

[[nodiscard]] auto
Occurrence::recurrenceModelFromJSON(const nlohmann::json &json)
    -> std::optional<recurrencePattern_t> {
    if (!json.contains("type")) {
        return std::nullopt;
    }

    if (auto type{json["type"].get<std::string>()}; type == "non-recurrent") {
        return NonRecurrent::fromJSON(json);
    } else if (type == "interval-recurrent") {
        return IntervalRecurrent::fromJSON(json);
    } else if (type == "weekday-recurrent") {
        return WeekdayRecurrent::fromJSON(json);
    }

    return std::nullopt;
}

[[nodiscard]] auto Occurrence::recurrenceModelToJSON(
    const recurrencePattern_t &recurrenceModel) const -> nlohmann::json {
    return std::visit(
        [](const auto &type) -> nlohmann::json { return type.toJSON(); },
        recurrenceModel_);
}

[[nodiscard]] auto Occurrence::getDateTime() const -> hbt::mods::DateTime {
    return datetime_;
}

[[nodiscard]] auto Occurrence::getRecurrenceModel() const
    -> recurrencePattern_t {
    return recurrenceModel_;
}

[[nodiscard]] auto Occurrence::getWeekday() const
    -> hbt::mods::DateTime::weekday_t {
    return datetime_.getWeekday();
}

[[nodiscard]] auto Occurrence::getInterval() const
    -> std::optional<hbt::mods::Interval> {
    if (const auto *itr{std::get_if<IntervalRecurrent>(&recurrenceModel_)}) {
        return itr->interval;
    }

    if (const auto *wdr{std::get_if<WeekdayRecurrent>(&recurrenceModel_)}) {
        return wdr->interval;
    }

    return std::nullopt;
}

[[nodiscard]] auto Occurrence::isRecurrent() const -> bool {
    return recurrenceModel_.index() != 0;
}

[[nodiscard]] auto Occurrence::toJSON() const -> nlohmann::json {
    return {{"datetime", datetime_.toISO8601String()},
            {"recurrence_model", recurrenceModelToJSON(recurrenceModel_)}};
}

[[nodiscard]] auto Occurrence::fromJSON(const nlohmann::json &json)
    -> std::optional<Occurrence> {
    if (!json.contains("datetime") || !json.contains("recurrence_model")) {
        return std::nullopt;
    }

    auto datetimeFromISO8601String{hbt::mods::DateTime::fromISO8601String(
        json["datetime"].get<std::string>())};
    if (!datetimeFromISO8601String.has_value()) {
        return std::nullopt;
    }

    auto recurrenceModel{recurrenceModelFromJSON(json["recurrence_model"])};
    if (!recurrenceModel.has_value()) {
        return std::nullopt;
    }

    return Occurrence{datetimeFromISO8601String.value(),
                      recurrenceModel.value()};
}

[[nodiscard]] auto Occurrence::isForDate(DateTime datetime) const -> bool {
    auto interval{getInterval()};

    if ((!interval.has_value() || interval->isZero()) &&
        datetime_ != datetime) {
        return false;
    }

    for (auto dateCopy{datetime_}; dateCopy <= datetime;
         dateCopy += interval.value()) {
        if (dateCopy == datetime) {
            return true;
        }
    }

    return false;
}

[[nodiscard]] auto Occurrence::operator==(const Occurrence &other) const
    -> bool {
    return datetime_ == datetime_ && recurrenceModel_ == other.recurrenceModel_;
}
} // namespace hbt::mods
