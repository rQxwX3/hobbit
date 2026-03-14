#include <occurence.hpp>

namespace hbt::mods {
enum class Month {
    JANUARY,
    FEBRUARY,
    MARCH,
    APRIL,
    MAY,
    JUNE,
    JULY,
    AUGUST,
    SEPTEMBER,
    OCTOBER,
    NOVEMBER,
    DECEMBER,
};

class Date {
  private:
    using year_t = int;
    using month_t = Month;
    using day_t = int;

  private:
    year_t year_;
    month_t month;
    day_t day;

  public:
    Date(year_t year, month_t month, day_t day);

  public:
    [[nodiscard]] static auto today() -> Date;

  public:
    [[nodiscard]] auto getYear() const -> year_t;

    [[nodiscard]] auto getMonth() const -> month_t;

    [[nodiscard]] auto getDay() const -> day_t;

    [[nodiscard]] auto getWeekday() const -> Weekday;

  public:
    [[nodiscard]] auto operator<=>(const Date &other) const -> bool = default;

  public:
    [[nodiscard]] auto isToday() const -> bool;
};
} // namespace hbt::mods
