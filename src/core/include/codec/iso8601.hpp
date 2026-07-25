#pragma once

#include <codec/codec.hpp>

namespace core::codec::iso8601 {
namespace concepts {
template <typename T, typename Model>
concept Concept = codec::concepts::Codec<T, Model, std::string> && requires {
    { T::regex } -> std::convertible_to<std::string_view>;
};
}; // namespace concepts

template <typename FT, FixedString Regex> struct Codec : codec::Codec<FT> {
    static constexpr auto regex{Regex};
};
}; // namespace core::codec::iso8601
