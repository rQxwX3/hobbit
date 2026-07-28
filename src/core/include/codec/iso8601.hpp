#pragma once

#include <codec/codec.hpp>

namespace core::codec::iso8601 {
namespace concepts {
template <typename T, typename Model>
concept Concept = requires {
    requires codec::concepts::Codec<T, Model, std::string>;

    { T::regex } -> std::convertible_to<std::string_view>;
};
}; // namespace concepts

template <typename FT, FixedString Regex> struct Codec : codec::Codec<FT> {
    static constexpr auto regex{std::string_view(Regex)};
};
}; // namespace core::codec::iso8601
