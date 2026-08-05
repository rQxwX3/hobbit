#pragma once

#include <error.hpp>
#include <fixed_string.hpp>

namespace clndr::dt::error::datetime {
using namespace core::error;

enum class Code : code_t {
    InvalidCtorArgs,
    FailedToValidateDate,
    FailedToValidateTime,
    ISO8601RegexMismatch,
    ISO8601UnitNotMatched,
    ISO8601InvalidDate,
    ISO8601InvalidTime,
};

static constexpr auto className{core::FixedString{"dt::DateTime"}};

using InvalidCtorArgs =
    Error<Code::InvalidCtorArgs, className,
          "cannot instantiate valid object from provided arguments">;

using FailedToValidateDate = Error<Code::FailedToValidateDate, className,
                                   "failed to validate provided Date">;

using FailedToValidateTime = Error<Code::FailedToValidateTime, className,
                                   "failed to validate provided Time">;

using ISO8601RegexMismatch = Error<Code::ISO8601RegexMismatch, className,
                                   "provided input doesn't match regex">;

using ISO8601UnitNotMatched =
    Error<Code::ISO8601UnitNotMatched, className,
          "provided input doesn't contain required unit(s)">;

using ISO8601InvalidDate = Error<Code::ISO8601InvalidDate, className,
                                 "provided input contains invalid Date">;

using ISO8601InvalidTime = Error<Code::ISO8601InvalidTime, className,
                                 "provided input contains invalid Time">;
}; // namespace clndr::dt::error::datetime
