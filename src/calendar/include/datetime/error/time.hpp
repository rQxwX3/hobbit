#pragma once

#include <error.hpp>
#include <fixed_string.hpp>

namespace clndr::dt::error::time {
using namespace core::error;

enum class Code : code_t {
    InvalidHour,
    InvalidMinute,
};

static constexpr auto className{core::FixedString{"dt::Time"}};

using InvalidHour = core::error::Error<
    Code::InvalidHour, std::invalid_argument, className,
    "cannot instantiate valid object with provided hour value">;

using InvalidMinute = core::error::Error<
    Code::InvalidMinute, std::invalid_argument, className,
    "cannot instantiate valid object with provided minute value">;
}; // namespace clndr::dt::error::time
