#pragma once

#include <error.hpp>
#include <fixed_string.hpp>

namespace clndr::ev::error::templ {
using namespace core::error;

enum class Code : code_t {
    InvalidTitle,
    InvalidRecurrenceDeadlineRelationship,
    InvalidRecurrenceEndDateTimeRelationship,
};

static constexpr auto className{core::FixedString{"ev::Template"}};

using InvalidTitle = Error<Code::InvalidTitle, std::invalid_argument, className,
                           "provided title is not valid (empty)">;

using InvalidRecurrenceDeadlineRelationship =
    Error<Code::InvalidRecurrenceDeadlineRelationship, std::invalid_argument,
          className,
          "invalid combination of rec::Recurrence and ev::Deadline values">;
}; // namespace clndr::ev::error::templ
