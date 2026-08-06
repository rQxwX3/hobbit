#pragma once

#include <fixed_string.hpp>

namespace core::error {
using code_t = uint8_t;

template <typename E>
concept Concept = requires {
    std::same_as<typename E::code, code_t>;
    std::convertible_to<typename E::msg, std::string>;
} && std::derived_from<E, std::exception>;

template <auto Code, typename Base, FixedString className, FixedString Message>
    requires std::derived_from<Base, std::exception>
struct Error : Base {
    static constexpr auto code{code_t(Code)};
    static constexpr auto msg{className + FixedString{": "} + Message};

    Error() : Base(std::string(msg)) {}
};
}; // namespace core::error
