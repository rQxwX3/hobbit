#pragma once

#include <algorithm>
#include <array>

namespace core {
template <std::size_t N> struct FixedString {
    std::array<char, N> value;

    constexpr FixedString(const std::array<char, N> &string) {
        std::copy_n(string, N, value);
    }
};
}; // namespace core
