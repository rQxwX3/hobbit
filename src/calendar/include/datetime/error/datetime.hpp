#pragma once

#include <error.hpp>
#include <fixed_string.hpp>

namespace clndr::dt::error::datetime {
using namespace core::error;

enum class Code : code_t {
    InvalidValue,
    FailedToValidateDate,
    FailedToValidateTime,
    ISO8601RegexMismatch,
    ISO8601UnitNotMatched,
    ISO8601InvalidDate,
    ISO8601InvalidTime,
};

static constexpr auto className{core::FixedString{"dt::DateTime"}};

using InvalidValue =
    Error<Code::InvalidValue, std::invalid_argument, className,
          "cannot instantiate valid object from provided value">;

using FailedToValidateDate =
    Error<Code::FailedToValidateDate, std::invalid_argument, className,
          "failed to validate provided Date">;

using FailedToValidateTime =
    Error<Code::FailedToValidateTime, std::invalid_argument, className,
          "failed to validate provided Time">;

using ISO8601RegexMismatch =
    Error<Code::ISO8601RegexMismatch, std::invalid_argument, className,
          "provided input doesn't match regex">;

using ISO8601UnitNotMatched =
    Error<Code::ISO8601UnitNotMatched, std::invalid_argument, className,
          "provided input doesn't contain required unit(s)">;

using ISO8601InvalidDate =
    Error<Code::ISO8601InvalidDate, std::invalid_argument, className,
          "provided input contains invalid dt::Date">;

using ISO8601InvalidTime =
    Error<Code::ISO8601InvalidTime, std::invalid_argument, className,
          "provided input contains invalid dt::Time">;
}; // namespace clndr::dt::error::datetime
