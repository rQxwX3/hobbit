#pragma once

#include <error.hpp>
#include <fixed_string.hpp>

namespace clndr::ev::error::instance {
using namespace core::error;

enum class Code : code_t {
    InvalidCtorArgs,
};

static constexpr auto className{core::FixedString("ev::Instance")};
}; // namespace clndr::ev::error::instance
