#pragma once

#include <error.hpp>
#include <fixed_string.hpp>

namespace clndr::ev::error::deadline {
using namespace core::error;

enum class Code : code_t {};

static constexpr auto className{core::FixedString("ev::Deadline")};
}; // namespace clndr::ev::error::deadline
