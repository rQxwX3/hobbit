#pragma once

#include <algorithm>
#include <string_view>

namespace core {
template <std::size_t N> struct FixedString {
    char value[N];

    constexpr FixedString() = default;

    constexpr FixedString(const char (&string)[N]) {
        std::copy_n(string, N, value);
    }

    constexpr operator std::string_view() const { return {value, N - 1}; }
};

template <std::size_t N1, std::size_t N2>
constexpr auto operator+(const FixedString<N1> &lhs,
                         const FixedString<N2> &rhs) {
    FixedString<N1 + N2 - 1> result{};

    for (std::size_t i = 0; i < N1 - 1; ++i) {
        result.value[i] = lhs.value[i];
    }

    for (std::size_t i = 0; i < N2; ++i) {
        result.value[N1 - 1 + i] = rhs.value[i];
    }

    return result;
};
}; // namespace core
