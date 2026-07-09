#include <recurrence/selected_weekdays.hpp>

namespace clndr::rec {
auto SelectedWeekdays::validateAndReverseString(std::string string)
    -> std::string {
    if (string.length() != dt::constants::weekdaysCount) {
        throw std::invalid_argument(
            errorMessage(Error::StringLengthExceedsLimit));
    }

    std::ranges::reverse(string);
    return string;
}

SelectedWeekdays::SelectedWeekdays(
    const std::vector<dt::constants::Weekday> &weekdays) {
    for (const auto wd : dt::constants::weekdaysArray) {
        days_.set(static_cast<size_t>(wd));
    }
}

SelectedWeekdays::SelectedWeekdays(days_t days) : days_{days} {}

[[nodiscard]] auto SelectedWeekdays::getDays() const -> days_t { return days_; }

[[nodiscard]] auto
SelectedWeekdays::containsWeekday(dt::constants::Weekday wd) const -> bool {
    return days_.test(static_cast<size_t>(wd));
}

[[nodiscard]] auto SelectedWeekdays::isEmpty() const -> bool {
    return std::ranges::none_of(
        dt::constants::weekdaysArray.begin(),
        dt::constants::weekdaysArray.end(),
        [this](const auto wd) -> bool { return this->containsWeekday(wd); });
}

auto SelectedWeekdays::addWeekday(dt::constants::Weekday wd) -> void {
    days_.set(static_cast<size_t>(wd));
}
} // namespace clndr::rec
