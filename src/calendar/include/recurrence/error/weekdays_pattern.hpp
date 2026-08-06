#pragma once

#include <error.hpp>
#include <fixed_string.hpp>

namespace clndr::rec::error::weekdays_pattern {
using namespace core::error;

enum class Code : code_t {
    InvalidCtorArgs,
    IntvalidInterval,
    InvalidSelectedWeekdays
};

static constexpr auto className{core::FixedString{"rec::WeekdaysPattern"}};

using InvalidCtorArgs =
    Error<Code::InvalidCtorArgs, className,
          "cannot instantiate valid object from provided arguments">;

using InvalidInterval =
    Error<Code::IntvalidInterval, className,
          "provided dt::Interval contains units other than week">;

using InvalidSelectedWeekdays =
    Error<Code::InvalidSelectedWeekdays, className,
          "cannot construct pattern from empty rec::SelectedWeekdays">;
}; // namespace clndr::rec::error::weekdays_pattern
