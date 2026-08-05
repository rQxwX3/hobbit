#pragma once

#include <error.hpp>
#include <fixed_string.hpp>

namespace clndr::dt::error::date {
using namespace core::error;

enum class Code : code_t {
    InvalidCtorArgs,
    InvalidInterval,
};

static constexpr auto className{core::FixedString("dt::Date")};

using InvalidCtorArgs =
    Error<Code::InvalidCtorArgs, className,
          "cannot instantiate valid object from provided arguments">;

using InvalidInterval = Error<Code::InvalidInterval, className,
                              "cannot perform addition with non-date interval">;
}; // namespace clndr::dt::error::date
