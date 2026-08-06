#pragma once

#include <fixed_string.hpp>

namespace core::error {
using code_t = uint8_t;

template <auto Code, typename Base, FixedString className, FixedString Message>
    requires std::derived_from<Base, std::exception>
struct Error : Base {
    static constexpr auto code{code_t(Code)};
    static constexpr auto msg{className + FixedString{": "} + Message};

    Error() : Base(std::string(msg)) {}
};
}; // namespace core::error
