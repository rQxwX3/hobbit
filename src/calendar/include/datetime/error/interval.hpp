#pragma once

#include <error.hpp>
#include <fixed_string.hpp>

namespace clndr::dt::error::interval {
using namespace core::error;

enum class Code : code_t {
    InvalidArray,
    InvalidMonthHandling,
    ISO8601FailedToParse,
    NaturalLanguageFailedToParse,
    InvalidComparison,
};

static constexpr auto className{core::FixedString{"dt::Interval"}};

using InvalidArray = Error<Code::InvalidArray, std::invalid_argument, className,
                           "provided dt::Interval::array_t value is invalid">;

using InvalidMonthHandling =
    Error<Code::InvalidMonthHandling, std::invalid_argument, className,
          "provided dt::Interval::MonthHandling value is invalid">;

using ISO8601FailedToParse =
    Error<Code::ISO8601FailedToParse, std::runtime_error, className,
          "failed to parse from ISO8601">;

using NaturalLanguageFailedToParse =
    Error<Code::NaturalLanguageFailedToParse, std::runtime_error, className,
          "failed to parse from natural language">;

using InvalidComparison =
    Error<Code::InvalidComparison, std::logic_error, className,
          "cannot compare intervals one of which contains both "
          "month and minute values after downward conversion">;
}; // namespace clndr::dt::error::interval
