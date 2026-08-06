#pragma once

#include <error.hpp>
#include <fixed_string.hpp>

namespace clndr::rec::error::interval_pattern {
using namespace core::error;

enum class Code : code_t {
    InvalidInterval,
};

static constexpr auto className{core::FixedString{"rec::IntervalPattern"}};

using InvalidInterval = Error<Code::InvalidInterval, className,
                              "cannot instantiate from zero Interval">;
}; // namespace clndr::rec::error::interval_pattern
