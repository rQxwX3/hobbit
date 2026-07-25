#pragma once

#include <concepts.hpp>

#include <fixed_string.hpp>

namespace core::codec::fields {
template <typename Type, FixedString Name, auto Validator> struct Field {
    using type = Type;

    static constexpr auto name{Name};
    static constexpr auto validator{Validator};
};

namespace concepts {
template <typename F>
concept Field = requires(F::type value) {
    { F::name };
    { F::validator(value) } -> std::same_as<bool>;
};

namespace impl {
template <typename FT> struct FieldTuple : std::bool_constant<false> {};

template <typename... Fs>
    requires(sizeof...(Fs) > 0)
struct FieldTuple<std::tuple<Fs...>>
    : std::bool_constant<(concepts::Field<Fs> && ...)> {};
} // namespace impl

template <typename FT>
concept FieldTuple = impl::FieldTuple<FT>::value;
}; // namespace concepts
}; // namespace core::codec::fields
