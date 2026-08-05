#pragma once

#include <error.hpp>
#include <fixed_string.hpp>

namespace clndr::dt::error::week {
using namespace core::error;

enum class Code : code_t {
    InvalidCtorArgs,
};

static constexpr auto className{core::FixedString{"dt::Week"}};

using InvalidCtorArgs =
    Error<Code::InvalidCtorArgs, className,
          "cannot instantiate valid object from provided arguments">;
}; // namespace clndr::dt::error::week
