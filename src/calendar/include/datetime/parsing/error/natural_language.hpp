#pragma once

#include <error.hpp>
#include <fixed_string.hpp>

namespace clndr::dt::parsing::error::natural_language {
using namespace core::error;

enum class Code : code_t {
    RegexMismatch,
    UnitBucketNotFound,
    RepeatedUnitParsing,
    InvalidUnitValue,
    FailedToParseUnit,
};

static constexpr auto className{
    core::FixedString("dt::parsing::NaturalLanguageParser")};

using RegexMismatch =
    Error<Code::RegexMismatch, className, "provided input didn't match regex">;

using UnitBucketNotFound = Error<Code::UnitBucketNotFound, className,
                                 "unit bucket(s) not initialized">;

using RepeatedUnitParsing =
    Error<Code::RepeatedUnitParsing, className,
          "provided input contains the same unit more than once">;

using InvalidUnitValue =
    Error<Code::InvalidUnitValue, className,
          "provided input contains invalid value (possible overflow)">;

using FailedToParseUnit = Error<Code::FailedToParseUnit, className,
                                "couldn't parse one or more units">;
}; // namespace clndr::dt::parsing::error::natural_language
