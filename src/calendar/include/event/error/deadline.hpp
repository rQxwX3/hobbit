#pragma once

#include <error.hpp>
#include <fixed_string.hpp>

namespace clndr::ev::error::deadline {
using namespace core::error;

enum class Code : code_t {
    InvalidCtorArgs,

    InvalidUnderlyingType,
    RTInvalidUnderlyingType,

    IntervalBadAccess,
    DateTimeBadAccess,
};

static constexpr auto className{core::FixedString("ev::Deadline")};

using InvalidCtorArgs =
    Error<Code::InvalidCtorArgs, className,
          "cannot instantiate valid object from provided arguments">;

using InvalidUnderlyingType =
    Error<Code::InvalidUnderlyingType, className,
          "provided value is not of a valid deadline type">;

using RTInvalidUnderlyingType =
    Error<Code::RTInvalidUnderlyingType, className,
          "invalid object state (invalid underlying type)">;

using IntervalBadAccess =
    Error<Code::IntervalBadAccess, className,
          "attempted to access interval in a non-interval instance">;

using DateTimeBadAccess =
    Error<Code::DateTimeBadAccess, className,
          "attempted to access datetime in a non-datetime instance">;
}; // namespace clndr::ev::error::deadline
