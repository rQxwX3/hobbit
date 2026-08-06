#pragma once

#include <error.hpp>
#include <fixed_string.hpp>

namespace clndr::ev::error::templ {
using namespace core::error;

enum class Code : code_t {
    InvalidTitle,
    InvalidDeadline,
    InvalidRecurrence,
    InvalidEndDateTime,
    InvalidStartDateTime,
};

static constexpr auto className{core::FixedString{"ev::Template"}};

using InvalidTitle =
    Error<Code::InvalidTitle, className, "provided title is not valid (empty)">;

using InvalidDeadline = Error<Code::InvalidDeadline, className,
                              "provided ev::Deadline is not valid/not "
                              "compatible object's rec::Recurrence">;

using InvalidRecurrence =
    Error<Code::InvalidRecurrence, className,
          "provided rec::Recurrence is not valid/not compatible with object's "
          "ev::Deadline">;

using InvalidEndDateTime =
    Error<Code::InvalidEndDateTime, className,
          "provided dt::OptDateTime end datetime is not valid/not compatible "
          "with object's dt::DateTime start datetime">;

using InvalidStartDateTime =
    Error<Code::InvalidStartDateTime, className,
          "provided dt::DateTime start datetime is not valid/not compatible "
          "with object's dt::OptDateTime end datetime">;
}; // namespace clndr::ev::error::templ
