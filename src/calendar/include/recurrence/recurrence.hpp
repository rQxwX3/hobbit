#pragma once

#include <datetime/datetime.hpp>
#include <datetime/opt_datetime.hpp>
#include <error.hpp>
#include <recurrence/interval_pattern.hpp>
#include <recurrence/null_pattern.hpp>
#include <recurrence/weekdays_pattern.hpp>

#include <nlohmann/json.hpp>

#include <variant>

namespace clndr::rec {
class Recurrence {
  public:
    using pattern_t =
        std::variant<NullPattern, IntervalPattern, WeekdaysPattern>;

  public:
    enum class PatternType : uint8_t {
        Null,
        Interval,
        Weekdays,
    };

  private:
    pattern_t pattern_;
    dt::DateTime startDateTime_;
    dt::OptDateTime endDateTime_;

  public:
    Recurrence(pattern_t pattern, dt::DateTime startDateTime,
               dt::OptDateTime endDateTime);

  public:
    [[nodiscard]] auto ok() const -> bool;

    template <typename Field> [[nodiscard]] auto fieldOK() const -> bool;

  public:
    [[nodiscard]] static auto
    null(dt::DateTime startDateTime = dt::DateTime::now()) -> Recurrence;

  public:
    [[nodiscard]] auto getDateTimesOfDate(dt::Date date) const
        -> std::vector<dt::DateTime>;

    [[nodiscard]] auto happensOnDate(dt::Date date) const -> bool;

  public:
    [[nodiscard]] auto getPatternType() const -> PatternType;

    [[nodiscard]] auto getPattern() const -> pattern_t;

    [[nodiscard]] auto getStartDateTime() const -> dt::DateTime;

    [[nodiscard]] auto getEndDateTime() const -> dt::OptDateTime;

  public:
    auto setPatternType(pattern_t pattern) -> void;

    auto setStartDateTime(dt::DateTime startDateTime) -> void;

    auto setEndDateTime(dt::OptDateTime endDateTime) -> void;

  public:
    [[nodiscard]] auto isIntervalPattern() const -> bool;

    [[nodiscard]] auto isWeekdaysPattern() const -> bool;

    [[nodiscard]] auto isNullPattern() const -> bool;

  public:
    [[nodiscard]] auto getNullPattern() const -> NullPattern;

    [[nodiscard]] auto getIntervalPattern() const -> IntervalPattern;

    [[nodiscard]] auto getWeekdaysPattern() const -> WeekdaysPattern;

  public:
    [[nodiscard]] auto operator==(const Recurrence &recurrence) const
        -> bool = default;
};
} // namespace clndr::rec
