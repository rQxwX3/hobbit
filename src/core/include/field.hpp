#pragma once

#include <fixed_string.hpp>

namespace core {
template <typename T, core::FixedString JSONName> struct Field {
    using type = T;

    static constexpr auto jsonName = JSONName;
};
} // namespace core
