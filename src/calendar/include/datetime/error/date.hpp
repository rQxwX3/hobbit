#pragma once

#include <error.hpp>
#include <fixed_string.hpp>

namespace clndr::dt::error::date {
using namespace core::error;

enum class Code : code_t {
    InvalidYearMonthDay,
    InvalidInterval,
};

static constexpr auto className{core::FixedString("dt::Date")};

using InvalidYearMonthDay =
    Error<Code::InvalidYearMonthDay, std::invalid_argument, className,
          "cannot instantiate valid object with provided Year-Month-Day value">;

using InvalidInterval =
    Error<Code::InvalidInterval, std::invalid_argument, className,
          "cannot perform addition with time-based interval">;
}; // namespace clndr::dt::error::date
