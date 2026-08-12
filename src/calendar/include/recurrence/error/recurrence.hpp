#pragma once

#include <error.hpp>
#include <fixed_string.hpp>

namespace clndr::rec::error::recurrence {
using namespace core::error;

enum class Code : code_t {
    InvalidPattern,
    InvalidStart,
    InvalidEnd,
    InvalidStartEndRelationship,
};

static constexpr auto className{core::FixedString{"rec::Recurrence"}};

using InvalidPattern = Error<Code::InvalidPattern, std::invalid_argument,
                             className, "provided rec::Pattern is invalid">;

using InvalidStart =
    Error<Code::InvalidStart, std::invalid_argument, className,
          "provided dt::DateTime object is not a valid start datetime">;

using InvalidEnd =
    Error<Code::InvalidStart, std::invalid_argument, className,
          "provided dt::DateTime object is not a valid end datetime">;

using InvalidStartEndRelationship =
    Error<Code::InvalidStartEndRelationship, std::invalid_argument, className,
          "invalid start-end datetime relationship">;
}; // namespace clndr::rec::error::recurrence
