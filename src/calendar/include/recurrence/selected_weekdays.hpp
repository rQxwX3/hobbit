#pragma once

#include <bitset>
#include <expected>
#include <string>

#include <datetime/week.hpp>

#include <nlohmann/json.hpp>

namespace clndr::rec {
class SelectedWeekdays {
  public:
    enum class Error : uint8_t {
        StringLengthExceedsLimit,
    };

  public:
    [[nodiscard]] static constexpr auto errorMessage(Error error)
        -> std::string {
        switch (error) {
        case Error::StringLengthExceedsLimit:
            return "Week: provided string exceeds maximum length";

        default:
            std::unreachable();
        }
    }

  public:
    using days_t = std::bitset<dt::constants::weekdaysCount>;

  private:
    days_t days_;

  public:
    [[nodiscard]] auto operator==(const SelectedWeekdays &) const
        -> bool = default;

  public:
    SelectedWeekdays(days_t days = days_t{});

    SelectedWeekdays(const std::vector<dt::constants::Weekday> &weekdays);

  public:
    [[nodiscard]] auto getDays() const -> days_t;

    [[nodiscard]] auto contains(dt::constants::Weekday wd) const -> bool;

    [[nodiscard]] auto isEmpty() const -> bool;

  public:
    auto addWeekday(dt::constants::Weekday wd) -> void;

  public:
    static auto validateAndReverseString(std::string string) -> std::string;
};
} // namespace clndr::rec
