#pragma once

#include <datetime/datetime.hpp>
#include <datetime/opt_datetime.hpp>
#include <error.hpp>
#include <recurrence/interval_pattern.hpp>
#include <recurrence/null_pattern.hpp>
#include <recurrence/weekdays_pattern.hpp>

#include <nlohmann/json.hpp>

#include <array>
#include <expected>
#include <string_view>
#include <variant>

namespace clndr::rec {
class Recurrence {
  public:
    using pattern_t =
        std::variant<NullPattern, IntervalPattern, WeekdaysPattern>;

  private:
    struct Error : core::err::Base<Error> {
        static constexpr auto className{std::string_view{"rec::Recurrence"}};

        enum class Code : uint8_t {
            UnsupportedPatternType,

            EndBeforeStart,
            StartAfterEnd,
        };

        [[nodiscard]] static constexpr auto errorMessage(Code code)
            -> std::string {
            switch (code) {
            case Code::UnsupportedPatternType:
                return generateMessage(
                    "invalid object state (unsupported pattern type)");

            case Code::EndBeforeStart:
                return generateMessage(
                    "end DateTime cannot appear before start DateTime");

            case Code::StartAfterEnd:
                return generateMessage(
                    "start DateTime cannot appear after end DateTime");

            default:
                std::unreachable();
            }
        }
    };

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
    [[nodiscard]] static auto
    null(dt::DateTime startDateTime = dt::DateTime::now()) -> Recurrence;

  public:
    [[nodiscard]] auto getDateTimesOfDate(dt::Date date) const
        -> std::vector<dt::DateTime>;

    [[nodiscard]] auto happensOnDate(dt::Date date) const -> bool;

  public:
    [[nodiscard]] auto getPatternType() const -> PatternType;

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
    [[nodiscard]] auto getIntervalPattern() const -> IntervalPattern;

    [[nodiscard]] auto getWeekdaysPattern() const -> WeekdaysPattern;

  public:
    [[nodiscard]] auto operator==(const Recurrence &recurrence) const
        -> bool = default;

  public:
    struct Validator {
        struct Validated {};

        static auto endAfterStart(dt::OptDateTime end, dt::DateTime start)
            -> void;

        struct Return {
            [[nodiscard]] static auto endAfterStart(dt::OptDateTime end,
                                                    dt::DateTime start)
                -> dt::OptDateTime;

            [[nodiscard]] static auto startBeforeEnd(dt::DateTime start,
                                                     dt::OptDateTime end)
                -> dt::DateTime;
        };
    };

  public:
    Recurrence(Validator::Validated, pattern_t pattern,
               dt::DateTime startDateTime, dt::OptDateTime endDateTime);

    [[nodiscard]] static auto fromValidated(pattern_t pattern,
                                            dt::DateTime startDateTime,
                                            dt::OptDateTime endDateTime)
        -> Recurrence;
};
} // namespace clndr::rec
