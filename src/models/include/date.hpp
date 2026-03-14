#include <chrono>

namespace hbt::mods {
class Date {
  private:
    std::chrono::year_month_day ymd_;

  public:
    Date(std::chrono::year_month_day ymd);

  public:
    [[nodiscard]] static auto today() -> Date;

  public:
    [[nodiscard]] auto getYear() const -> std::chrono::year;

    [[nodiscard]] auto getMonth() const -> std::chrono::month;

    [[nodiscard]] auto getDay() const -> std::chrono::day;

    [[nodiscard]] auto getWeekday() const -> std::chrono::weekday;

  public:
    [[nodiscard]] auto operator<=>(const Date &other) const -> bool = default;

  public:
    [[nodiscard]] auto isToday() const -> bool;
};
} // namespace hbt::mods
