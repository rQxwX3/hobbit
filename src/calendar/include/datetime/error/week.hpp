#pragma once

#include <error.hpp>
#include <fixed_string.hpp>

namespace clndr::dt::error::week {
using namespace core::error;

enum class Code : code_t {
    InvalidArray,
};

static constexpr auto className{core::FixedString{"dt::Week"}};

using InvalidArray =
    Error<Code::InvalidArray, std::invalid_argument, className,
          "cannot instantiate valid object with provided dt::Date array">;
}; // namespace clndr::dt::error::week
