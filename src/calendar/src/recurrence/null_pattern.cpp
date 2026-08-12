#include <recurrence/null_pattern.hpp>

namespace clndr::rec {
[[nodiscard]] auto NullPattern::getType() const -> pattern::Type {
    return type;
}

[[nodiscard]] auto NullPattern::getOccurrencesOfDate(dt::Date date,
                                                     dt::DateTime start) const
    -> occurrences_t {
    if (happensOnDate(date, start)) {
        return {date};
    }

    return {};
}

[[nodiscard]] auto NullPattern::happensOnDate(dt::Date date,
                                              dt::DateTime start) const
    -> bool {
    return date == start;
}
} // namespace clndr::rec
