#pragma once

#include <error.hpp>
#include <fixed_string.hpp>

namespace clndr::dt::parsing::error::iso8601 {
using namespace core::error;

enum class Code : code_t {
    RegexMismatch,
    InvalidUnitValue,
};

static constexpr auto className{
    core::FixedString("dt::parsing::ISO8601IntervalParser")};

using RegexMismatch =
    Error<Code::RegexMismatch, className, "provided input didn't match regex">;

using InvalidUnitValue =
    Error<Code::InvalidUnitValue, className,
          "provided input contains invalid value (possible overflow)">;
}; // namespace clndr::dt::parsing::error::iso8601
