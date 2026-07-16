#pragma once

#include <algorithm>

namespace core {
template <std::size_t N> struct FixedString {
    char value[N];

    constexpr FixedString(const char (&string)[N]) {
        std::copy_n(string, N, value);
    }
};
}; // namespace core
