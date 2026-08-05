#pragma once

#include <fixed_string.hpp>

namespace core::error {
using code_t = uint8_t;

template <auto Code, FixedString className, FixedString Message> struct Error {
    static constexpr auto code{code_t(Code)};
    static constexpr auto msg{className + FixedString{": "} + Message};
};
}; // namespace core::error
