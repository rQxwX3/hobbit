#pragma once

#include <datetime/week.hpp>
#include <recurrence/pattern.hpp>
#include <recurrence/selected_weekdays.hpp>

namespace clndr::rec {
class WeekdaysPattern : public Pattern {
  public:
    struct Error {
        static constexpr auto className{
            std::string_view{"recurrence::WeekdaysPattern"}};

        enum class Code : uint8_t {
            InvalidInterval,
            EmptyWeek,
        };

        [[nodiscard]] static constexpr auto
        generateMessage(const std::string &message) -> std::string {
            return static_cast<std::string>(className) + ": " + message;
        }

        [[nodiscard]] static constexpr auto getMessageForCode(Code code)
            -> std::string {
            switch (code) {
            case Code::InvalidInterval:
                return "recurrence::WeekdayPattern: provided Interval contains "
                       "units other than week";

            case Code::EmptyWeek:
                return "recurrence::WeekdayPattern: cannot construct pattern "
                       "from "
                       "empty Week object";

            default:
                std::unreachable();
            }
        }
    };

  private:
    /* don't change the order */
    dt::Week firstWeek_;
    dt::Interval interval_;
    SelectedWeekdays selectedWeekdays_;

  public:
    WeekdaysPattern(dt::Date startDate, rec::SelectedWeekdays selectedWDs,
                    dt::Interval interval);

    WeekdaysPattern(dt::Week firstWeek, rec::SelectedWeekdays selectedWDs,
                    dt::Interval interval);

  public:
    [[nodiscard]] auto
    getOccurrencesOfDate(dt::Date date,
                         dt::DateTime start = dt::DateTime::now()) const
        -> occurrences_t override;

    [[nodiscard]] auto
    happensOnDate(dt::Date date, dt::DateTime start = dt::DateTime::now()) const
        -> bool override;

  public:
    static auto validateInterval(const dt::Interval &interval) -> dt::Interval;

    static auto validateSelectedWeekdays(const SelectedWeekdays &selectedWDs)
        -> SelectedWeekdays;

  private:
    [[nodiscard]] static auto getFirstOccurrence(dt::Date startDate,
                                                 SelectedWeekdays selectedWDs)
        -> dt::DateTime;

  public:
    [[nodiscard]] auto getInterval() const -> dt::Interval;

    [[nodiscard]] auto getFirstWeek() const -> dt::Week;

    [[nodiscard]] auto getSelectedWeekdays() const -> SelectedWeekdays;
};
} // namespace clndr::rec
