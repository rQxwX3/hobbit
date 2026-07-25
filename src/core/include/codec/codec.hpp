#pragma once

#include <concepts>
#include <expected>

#include <codec/fields.hpp>
#include <error.hpp>

namespace core::codec {
namespace concepts {
template <typename T, typename Model, typename Representation>
concept Codec =
    err::Concept<typename T::Error> && requires(Model obj, Representation rep) {
        requires fields::concepts::FieldTuple<typename T::fields, Model>;

        { T::encode(obj) } -> std::same_as<Representation>;
        {
            T::decode(rep)
        } -> std::same_as<std::expected<Model, typename T::Error::Code>>;
    };

}; // namespace concepts

template <typename FT> struct Codec {
    using fields = FT;
};
}; // namespace core::codec
