#pragma once

#include <string_view>
#include <tuple>

#include <concepts.hpp>
#include <fixed_string.hpp>
#include <model.hpp>

namespace core::schema::fields {
namespace concepts {
template <typename F, typename Model>
concept Field = requires(const Model &obj) {
    { F::accessor(obj) } -> std::same_as<typename F::type>;

    { F::name } -> std::same_as<std::string_view>;

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

template <typename Model, typename T, auto Accessor, core::FixedString Name>
struct Field {
    static constexpr auto accessor{Accessor};
    static constexpr auto name{Name};

    using type = T;
};
} // namespace core::schema::fields
