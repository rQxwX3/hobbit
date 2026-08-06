#pragma once

#include <error.hpp>
#include <fixed_string.hpp>

namespace clndr::rec::error::recurrence {
using namespace core::error;

enum class Code : code_t {
    InvalidCtorArgs,
    InvalidPattern,
    InvalidStart,
    InvalidEnd,
    InvalidStartEndRelationship,
};

static constexpr auto className{core::FixedString{"rec::Recurrence"}};

using InvalidCtorArgs =
    Error<Code::InvalidCtorArgs, className,
          "cannot instantiate valid object from provided arguments">;

using InvalidPattern =
    Error<Code::InvalidCtorArgs, className, "provided rec::Pattern is invalid">;

using InvalidStart =
    Error<Code::InvalidStart, className,
          "provided dt::DateTime object is not a valid start datetime">;

using InvalidEnd =
    Error<Code::InvalidStart, className,
          "provided dt::DateTime object is not a valid end datetime">;

using InvalidStartEndRelationship =
    Error<Code::InvalidStartEndRelationship, className,
          "invalid start-end datetime relationship">;
}; // namespace clndr::rec::error::recurrence
