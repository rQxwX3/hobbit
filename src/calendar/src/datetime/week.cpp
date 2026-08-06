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

    if (!ok()) {
        throw std::invalid_argument(
            std::string(error::week::InvalidCtorArgs::msg));
    }
}

Week::Week(array_t array) : array_{array} {
    if (!ok()) {
        throw std::invalid_argument(
            std::string(error::week::InvalidCtorArgs::msg));
    }
}

[[nodiscard]] auto Week::ok() const -> bool {
    return schema::week::Schema::validate(*this);
}

[[nodiscard]] auto Week::toArray() const -> array_t { return array_; }

[[nodiscard]] auto Week::operator[](constants::Weekday wd) const -> Date {
    return array_[static_cast<size_t>(wd)];
}
}; // namespace clndr::dt
