#include <datetime/datetime.hpp>
#include <datetime/error/week.hpp>
#include <datetime/interval.hpp>
#include <datetime/schema/week.hpp>
#include <datetime/week.hpp>

namespace clndr::dt {
Week::Week(Date date) {
    auto dt{DateTime(date)};

    for (auto i{0}; i != constants::weekdaysCount; ++i) {
        array_[static_cast<size_t>(dt.getWeekday())] = date;
        dt += Interval::days(1);
    }

    schema::week::Schema::validateAllRules(*this);
}

Week::Week(array_t array) : array_{array} {
    schema::week::Schema::validateAllRules(*this);
}

[[nodiscard]] auto Week::getArray() const -> array_t { return array_; }

[[nodiscard]] auto Week::operator[](constants::Weekday wd) const -> Date {
    return array_[static_cast<size_t>(wd)];
}
}; // namespace clndr::dt
