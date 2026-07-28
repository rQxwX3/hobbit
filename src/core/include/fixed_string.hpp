#pragma once

#include <algorithm>
#include <string_view>

namespace core {
template <std::size_t N> struct FixedString {
    char value[N];

    constexpr FixedString(const char (&string)[N]) {
        std::copy_n(string, N, value);
    }

    constexpr operator std::string_view() const { return {value, N - 1}; }
};
}; // namespace core
