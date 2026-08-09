#pragma once

#include <error.hpp>
#include <fixed_string.hpp>

namespace clndr::dt::error::opt_datetime {
using namespace core::error;

enum class Code : code_t {
    InvalidValue,
};

static constexpr auto className{core::FixedString{"dt::OptDateTime"}};

using InvalidValue = Error<Code::InvalidValue, std::invalid_argument, className,
                           "provided value is not a valid dt::DateTime object">;
}; // namespace clndr::dt::error::opt_datetime
