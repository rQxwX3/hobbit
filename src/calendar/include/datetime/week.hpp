#pragma once

#include <array>

#include <datetime/constants.hpp>
#include <datetime/date.hpp>

namespace clndr::dt {
class Week {
  public:
    using array_t = std::array<Date, constants::weekdaysCount>;

  private:
    array_t array_;

  public:
    Week(Date date);

    Week(array_t array);

  public:
    [[nodiscard]] auto toArray() const -> array_t;

  public:
    [[nodiscard]] auto operator[](constants::Weekday wd) -> Date &;

    [[nodiscard]] auto operator[](constants::Weekday wd) const -> Date;
};
} // namespace clndr::dt
