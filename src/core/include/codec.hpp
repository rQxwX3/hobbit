#pragma once

#include <concepts>
#include <expected>

#include <error.hpp>

namespace core::codec {
template <typename T, typename Object, typename Representation>
concept Concept =
    err::Concept<typename T::Error> &&
    requires(Object obj, Representation rep) {
        { T::encode(obj) } -> std::same_as<Representation>;
        {
            T::decode(rep)
        } -> std::same_as<std::expected<Object, typename T::Error::Code>>;
    };
}; // namespace core::codec
