#pragma once

#include <error.hpp>
#include <recurrence/pattern.hpp>

namespace clndr::rec {
class IntervalPattern : public Pattern {
  public:
    struct Error : core::err::Base<Error> {
        static constexpr auto className{
            std::string_view{"recurrence::IntervalPattern"}};

        enum class Code : uint8_t {
            InvalidInterval,
        };

        [[nodiscard]] static constexpr auto getMessageForCode(Code code)
            -> std::string {
            switch (code) {
            case Code::InvalidInterval:
                return generateMessage("cannot instantiate from zero "
                                       "Interval");

            default:
                std::unreachable();
            }
        }
    };

  public:
    static auto validateInterval(const dt::Interval &interval) -> dt::Interval;

  private:
    dt::Interval interval_;

  public:
    IntervalPattern(dt::Interval interval);

  public:
    [[nodiscard]] auto getOccurrencesOfDate(dt::Date date,
                                            dt::DateTime start) const
        -> occurrences_t override;

    [[nodiscard]] auto happensOnDate(dt::Date date, dt::DateTime start) const
        -> bool override;

  public:
    [[nodiscard]] auto getInterval() const -> dt::Interval;

  public:
    [[nodiscard]] auto getFirstOccurrenceOfDate(dt::Date date,
                                                dt::DateTime start) const
        -> std::optional<occurrence_t>;

  public:
    [[nodiscard]] auto operator==(const IntervalPattern &other) const -> bool;
};
} // namespace clndr::rec
