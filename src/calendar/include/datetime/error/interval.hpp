#pragma once

#include <error.hpp>
#include <fixed_string.hpp>

namespace clndr::dt::error::interval {
using namespace core::error;

enum class Code : code_t {
    InvalidCtorArgs,
    InvalidMonthHandling,
    ISO8601FailedToParse,
    NaturalLanguageFailedToParse,
    InvalidComparison,
};

static constexpr auto className{core::FixedString{"dt::Interval"}};

using InvalidCtorArgs =
    Error<Code::InvalidCtorArgs, className,
          "cannot instantiate valid object from provided arguments">;

using InvalidMonthHandling = Error<Code::InvalidMonthHandling, className,
                                   "provided month handling is invalid">;

using ISO8601FailedToParse = Error<Code::ISO8601FailedToParse, className,
                                   "failed to parse from ISO8601">;

using NaturalLanguageFailedToParse =
    Error<Code::NaturalLanguageFailedToParse, className,
          "failed to parse from natural language">;

using InvalidComparison =
    Error<Code::InvalidComparison, className,
          "cannot compare intervals if one of them contains both "
          "months and minutes after downward conversion">;
}; // namespace clndr::dt::error::interval
