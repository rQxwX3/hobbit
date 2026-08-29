#pragma once

#include <concepts>
#include <expected>

#include <codec/fields.hpp>
#include <error.hpp>

namespace core::codec {
namespace concepts {
namespace impl {
template <typename Field, typename Codec>
concept FieldProcessedByCodec =
    requires(Codec::model model, Codec::representation representation) {
        {
            Codec::template encode<Field>(model)
        } -> std::same_as<typename Field::representationType>;

        {
            Codec::template decode<Field>(representation)
        } -> std::same_as<typename Field::modelType>;
    };

template <typename Codec, typename... Fields>
consteval auto fieldsProcessedByCodec(std::tuple<Fields...> * /*unused*/)
    -> bool {
    return (FieldProcessedByCodec<Fields, Codec> && ...);
}

template <typename Codec>
concept ProcessesEachField = impl::fieldsProcessedByCodec<Codec>(
    static_cast<typename Codec::fields *>(nullptr));
}; // namespace impl

template <typename T, typename Model, typename Representation>
concept Codec = requires(Model obj, Representation format) {
    { T::model } -> std::same_as<Model>;
    { T::representation } -> std::same_as<Representation>;

    { T::encode(obj) } -> std::same_as<typename T::representation>;
    {
        T::decode(format)
    }
    -> std::same_as<std::expected<typename T::model, typename T::Error::Code>>;
} && impl::ProcessesEachField<T>;
}; // namespace concepts
}; // namespace core::codec
