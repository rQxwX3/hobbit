#pragma once

#include <concepts>

#include <concepts.hpp>
#include <schema/fields.hpp>

namespace core::schema::rules {
namespace concepts {
template <typename R, typename Model, typename... Fields>
concept Rule = requires(const Model &obj) {
    requires core::concepts::NonEmptyTuple<typename R::fields>;
    requires core::concepts::TupleSubset<typename R::fields,
                                         std::tuple<Fields...>>;

    { R::check(obj) } -> std::same_as<bool>;
};

namespace impl {
template <typename RT, typename Model, typename Fields>
struct RuleTuple : std::bool_constant<false> {};

template <typename... Rs, typename Model, typename... Fields>
struct RuleTuple<std::tuple<Rs...>, Model, std::tuple<Fields...>>
    : std::bool_constant<(Rule<Rs, Model, Fields...> && ...)> {};
} // namespace impl

template <typename Rule, typename Field>
concept RuleContainsField =
    core::concepts::TupleContains<Field, typename Rule::fields>;

template <typename RT, typename Model, typename Fields>
concept RuleTuple = impl::RuleTuple<RT, Model, Fields>::value;
} // namespace concepts

template <typename Fields> struct Rule {
    using fields = Fields;
};
} // namespace core::schema::rules
