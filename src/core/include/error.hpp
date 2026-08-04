#pragma once

#include <fixed_string.hpp>

namespace core::error {
template <FixedString className, FixedString Message> struct Error {
    static constexpr auto msg{className + FixedString{": "} + Message};
};
}; // namespace core::error
