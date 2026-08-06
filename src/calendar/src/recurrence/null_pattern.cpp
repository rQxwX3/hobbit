#include <recurrence/null_pattern.hpp>

namespace clndr::rec {
[[nodiscard]] auto NullPattern::getOccurrencesOfDate(dt::Date date,
                                                     dt::DateTime start) const
    -> occurrences_t {
    if (happensOnDate(date, start)) {
        return {date};
    }

    return {};
}

[[nodiscard]] auto NullPattern::ok() const -> bool { return true; }

[[nodiscard]] auto NullPattern::happensOnDate(dt::Date date,
                                              dt::DateTime start) const
    -> bool {
    return date == start;
}
} // namespace clndr::rec
