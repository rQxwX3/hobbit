#pragma once

#include <chrono>
#include <variant>

namespace hbt::mods {
template <typename T>
concept SupportedDuration = std::is_same_v<T, std::chrono::minutes> ||
                            std::is_same_v<T, std::chrono::hours> ||
                            std::is_same_v<T, std::chrono::days> ||
                            std::is_same_v<T, std::chrono::weeks> ||
                            std::is_same_v<T, std::chrono::months> ||
                            std::is_same_v<T, std::chrono::years>;

class Interval {
  private:
    using duration_t = std::variant<std::chrono::minutes, std::chrono::hours,
                                    std::chrono::days, std::chrono::weeks,
                                    std::chrono::months, std::chrono::years>;

  private:
    duration_t duration_;

  public:
    Interval() = delete;

    template <SupportedDuration T>
    explicit Interval(T duration) : duration_{duration} {}

  public:
    [[nodiscard]] auto toMinutes() -> std::chrono::minutes;

    [[nodiscard]] auto toHours() -> std::chrono::hours;

    [[nodiscard]] auto toDays() -> std::chrono::days;

    [[nodiscard]] auto toWeeks() -> std::chrono::weeks;

    [[nodiscard]] auto toMonths() -> std::chrono::months;

    [[nodiscard]] auto toYears() -> std::chrono::years;

    template <typename Rep, typename Period>
    [[nodiscard]] auto getDuration() -> std::chrono::duration<Rep, Period> {
        return duration_;
    }
};
} // namespace hbt::mods
