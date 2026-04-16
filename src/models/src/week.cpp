#include <week.hpp>

namespace hbt::mods {
Week::Week(const std::vector<Date::weekday_t> &weekdays) {
    for (const auto wd : weekdays) {
        days_.set(static_cast<size_t>(wd));
    }
}

Week::Week(const std::string &daysString) : days_{days_t(daysString)} {}

[[nodiscard]] auto Week::getDays() const -> days_t { return days_; }
}; // namespace hbt::mods
