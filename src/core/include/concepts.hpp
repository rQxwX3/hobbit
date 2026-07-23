#pragma once

#include <tuple>

namespace core::concepts {
namespace impl {
template <typename T, typename Tuple>
struct TupleContains : std::bool_constant<false> {};

template <typename T, typename... Ts>
struct TupleContains<T, std::tuple<Ts...>>
    : std::bool_constant<(std::same_as<T, Ts> || ...)> {};

template <typename Subset, typename Superset>
struct TupleSubset : std::bool_constant<false> {};

template <typename... Ts, typename Superset>
struct TupleSubset<std::tuple<Ts...>, Superset>
    : std::bool_constant<(TupleContains<Ts, Superset>::value && ...)> {};

template <typename T> struct NonEmptyTuple : std::bool_constant<false> {};

template <typename... Ts>
struct NonEmptyTuple<std::tuple<Ts...>>
    : std::bool_constant<(sizeof...(Ts) > 0)> {};
} // namespace impl

template <typename Tuple, typename T>
concept TupleContains = impl::TupleContains<T, Tuple>::value;

template <typename Subset, typename Superset>
concept TupleSubset = impl::TupleSubset<Subset, Superset>::value;

template <typename Tuple>
concept NonEmptyTuple = impl::NonEmptyTuple<Tuple>::value;
}; // namespace core::concepts
