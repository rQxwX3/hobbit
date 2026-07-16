#pragma once

#include <chrono>
#include <cstdint>

#include <datetime/constants.hpp>
#include <datetime/interval.hpp>

#include <model.hpp>

namespace clndr::dt {
class Date {
  public:
    using year_t = int16_t;
    using month_t = uint8_t;
    using day_t = uint8_t;

    using duration_t = std::chrono::days;

  public:
    enum class Error : uint8_t {
        InvalidInterval,
        MissingFieldTypeSpecialization
    };

  public:
    [[nodiscard]] static constexpr auto errorMessage(Error error)
        -> std::string {
        switch (error) {
        case Error::InvalidInterval:
            return "Date: cannot perform addition with non-date interval";

        default:
            std::unreachable();
        }
    }

  private:
    /* order must not be changed */
    year_t year_;
    month_t month_;
    day_t day_;

  public:
    // struct Schema {
    //     enum class FieldID : core::FieldID { year, month, day, count_ };
    //
    //     using fields = core::Fields<
    //         core::Field<year_t, static_cast<core::FieldID>(FieldID::year),
    //                     "year">,
    //         core::Field<month_t, static_cast<core::FieldID>(FieldID::month),
    //                     "month">,
    //         core::Field<day_t, static_cast<core::FieldID>(FieldID::day),
    //                     "day">>;
    // };

  public:
    Date();

    Date(year_t year, month_t month, day_t day);

  public:
    [[nodiscard]] auto getYear() const -> year_t;

    [[nodiscard]] auto getMonth() const -> month_t;

    [[nodiscard]] auto getDay() const -> day_t;

  public:
    [[nodiscard]] auto ok() const -> bool;

    [[nodiscard]] auto toDuration() const -> duration_t;

    [[nodiscard]] auto toYMD() const -> std::chrono::year_month_day;

    [[nodiscard]] auto getWeekday() const -> constants::Weekday;

    [[nodiscard]] auto operator<=>(const Date &other) const
        -> std::strong_ordering = default;

    [[nodiscard]] auto operator+(const Interval &interval) const -> Date;

    auto operator+=(const Interval &interval) -> Date &;
};

// static_assert(core::model::Concept<Date>);
} // namespace clndr::dt
