#include <occurrence.hpp>

namespace hbt::mods {
Occurrence::Occurrence(hbt::mods::Date date, recurrenceModel_t recurrenceModel)
    : date_{date}, recurrenceModel_{std::move(recurrenceModel)} {}

Occurrence::Occurrence(hbt::mods::Date date,
                       const hbt::mods::Interval &interval)
    : date_{date}, recurrenceModel_{IntervalRecurrent{interval}} {}

Occurrence::Occurrence(hbt::mods::Date date, hbt::mods::Date::Week week,
                       const hbt::mods::Interval &interval)
    : date_{date} {
    if (!interval.onlyContainsUnit(DurationUnits::WEEK)) {
        throw std::invalid_argument(
            "Interval for weekday-recurrent occurrences must be in weeks");
    }

    if (week.getDays().none()) {
        throw std::invalid_argument("At least one weekday must be provided for "
                                    "weekday-reccurrent occurrences");
    }

    recurrenceModel_ = WeekdayRecurrent{.week = week, .interval = interval};
}

[[nodiscard]] auto
Occurrence::recurrenceModelFromJSON(const nlohmann::json &json)
    -> std::optional<recurrenceModel_t> {
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
    const recurrenceModel_t &recurrenceModel) const -> nlohmann::json {
    return std::visit(
        [](const auto &type) -> nlohmann::json { return type.toJSON(); },
        recurrenceModel_);
}

[[nodiscard]] auto Occurrence::getDate() const -> hbt::mods::Date {
    return date_;
}

[[nodiscard]] auto Occurrence::getRecurrenceModel() const -> recurrenceModel_t {
    return recurrenceModel_;
}

[[nodiscard]] auto Occurrence::getWeekday() const
    -> hbt::mods::Date::weekday_t {
    return date_.getWeekday();
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
    return {{"date", date_.toISO8601String()},
            {"recurrence_model", recurrenceModelToJSON(recurrenceModel_)}};
}

[[nodiscard]] auto Occurrence::fromJSON(const nlohmann::json &json)
    -> std::optional<Occurrence> {
    if (!json.contains("date") || !json.contains("recurrence_model")) {
        return std::nullopt;
    }

    auto dateFromISO8601String{
        hbt::mods::Date::fromISO8601String(json["date"].get<std::string>())};
    if (!dateFromISO8601String.has_value()) {
        return std::nullopt;
    }

    auto recurrenceModel{recurrenceModelFromJSON(json["recurrence_model"])};
    if (!recurrenceModel.has_value()) {
        return std::nullopt;
    }

    return Occurrence{dateFromISO8601String.value(), recurrenceModel.value()};
}

[[nodiscard]] auto Occurrence::isForDate(Date date) const -> bool {
    auto interval{getInterval()};

    if ((!interval.has_value() || interval->isZero()) && date_ != date) {
        return false;
    }

    for (auto dateCopy{date_}; dateCopy <= date; dateCopy += interval.value()) {
        if (dateCopy == date) {
            return true;
        }
    }

    return false;
}

[[nodiscard]] auto Occurrence::operator==(const Occurrence &other) const
    -> bool {
    return date_ == date_ && recurrenceModel_ == other.recurrenceModel_;
}
} // namespace hbt::mods
