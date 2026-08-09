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
    Week();

    Week(Date date);

    Week(array_t array);

  public:
    [[nodiscard]] auto getArray() const -> array_t;

  public:
    auto setArray(array_t array) -> void;

  public:
    [[nodiscard]] auto operator[](constants::Weekday wd) const -> Date;
};
} // namespace clndr::dt
