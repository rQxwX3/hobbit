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
    requires(Field::type value, Field::format representation) {
        {
            Codec::template encode<Field>(value)
        } -> std::same_as<typename Field::format>;

        {
            Codec::template decode<Field>(representation)
        } -> std::same_as<typename Field::type>;

        { Codec::format } -> std::same_as<typename Field::format>;
    };

template <typename Codec, typename... Fields>
consteval auto fieldsProcessedByCodec(std::tuple<Fields...> * /*unused*/)
    -> bool {
    return (FieldProcessedByCodec<Fields, Codec> && ...);
}
}; // namespace impl

template <typename Codec>
concept ProcessesEachField = impl::fieldsProcessedByCodec<Codec>(
    static_cast<typename Codec::fields *>(nullptr));

template <typename T, typename Model, typename Format>
concept Codec = requires(Model obj, Format format) {
    { T::encode(obj) } -> std::same_as<Format>;
    {
        T::decode(format)
    } -> std::same_as<std::expected<Model, typename T::Error::Code>>;
} && ProcessesEachField<T>;
}; // namespace concepts
}; // namespace core::codec
