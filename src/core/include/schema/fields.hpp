#pragma once

#include <tuple>

#include <concepts.hpp>

namespace core::schema::fields {
namespace concepts {
template <typename F, typename Model>
concept Field = requires(const Model &obj) {
    { F::accessor(obj) } -> std::same_as<typename F::type>;

    typename F::type;
};

namespace impl {
template <typename FT, typename Model>
struct FieldTuple : std::bool_constant<false> {};

template <typename... Fs, typename Model>
struct FieldTuple<std::tuple<Fs...>, Model>
    : std::bool_constant<(Field<Fs, Model> && ...)> {};
} // namespace impl

template <typename FT, typename Model>
concept FieldTuple = impl::FieldTuple<FT, Model>::value;
} // namespace concepts

template <typename T, typename Model, auto Accessor> struct Field {
    using type = T;
    using model = Model;

    static constexpr auto accessor{Accessor};
};

template <typename... Fs> using Fields = std::tuple<Fs...>;
} // namespace core::schema::fields
