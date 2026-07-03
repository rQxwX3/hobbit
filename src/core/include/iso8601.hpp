#pragma once

#include <concepts>
#include <regex>

#include <codec.hpp>

namespace core::codec::iso8601 {
template <typename T, typename Object>
concept Concept =
    codec::Concept<T, Object, std::string> &&
    requires(Object obj, std::string rep) {
        { T::regex } -> std::same_as<const std::regex &>;
        { T::encode(obj) } -> std::same_as<std::string>;
        {
            T::decode(rep)
        } -> std::same_as<std::expected<Object, typename T::Error::Code>>;
    };
}; // namespace core::codec::iso8601
