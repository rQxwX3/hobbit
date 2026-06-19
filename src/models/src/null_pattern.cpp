#include <null_pattern.hpp>

namespace hbt::mods::util {
[[nodiscard]] auto
NullRecurrencePattern::getOccurrencesOfDate(DateTime on) const
    -> occurrences_t {
    if (on == dateTime_) {
        return {on};
    }

    return {};
}
} // namespace hbt::mods::util
