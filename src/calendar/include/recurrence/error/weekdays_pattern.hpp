#pragma once

#include <error.hpp>
#include <fixed_string.hpp>

namespace clndr::rec::error::weekdays_pattern {
using namespace core::error;

enum class Code : code_t {
    InvalidFirstWeek,
    IntvalidInterval,
    InvalidSelectedWeekdays
};

static constexpr auto className{core::FixedString{"rec::WeekdaysPattern"}};

using InvalidFirstWeek =
    Error<Code::InvalidFirstWeek, std::runtime_error, className,
          "failed to validate dt::Week object used for firstWeek field">;

using InvalidInterval =
    Error<Code::IntvalidInterval, std::invalid_argument, className,
          "provided dt::Interval contains units other than week">;

using InvalidSelectedWeekdays =
    Error<Code::InvalidSelectedWeekdays, std::invalid_argument, className,
          "cannot construct pattern from empty rec::SelectedWeekdays">;
}; // namespace clndr::rec::error::weekdays_pattern
