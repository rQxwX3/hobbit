#pragma once

#include <concepts>
#include <schema/fields.hpp>

#include <schema/rules.hpp>

namespace core::schema {
namespace concepts {
template <typename S, typename Model>
concept Schema =
    std::same_as<typename S::Model, Model> && requires(const Model &obj) {
        requires core::schema::fields::concepts::FieldTuple<typename S::fields,
                                                            Model>;

        requires core::schema::rules::concepts::RuleTuple<
            typename S::rules, Model, typename S::fields>;

        typename S::RuleSet;

        { S::validate(obj) } -> std::same_as<bool>;
    };
}; // namespace concepts

namespace meta {
template <typename Field, typename Rules> struct RulesForField;

template <typename Field, typename... Rules>
struct RulesForField<Field, std::tuple<Rules...>> {
    template <typename Rule>
    using Match =
        std::conditional_t<rules::concepts::RuleContainsField<Rule, Field>,
                           std::tuple<Rule>, std::tuple<>>;

    template <typename... Ts> struct Join;

    template <typename... Ts> struct Join<std::tuple<Ts...>> {
        using type = std::tuple<Ts...>;
    };

    using type = typename Join<Match<Rules>...>::type;
};
} // namespace meta

template <typename Derived, typename Model> struct Base {
    [[nodiscard]] static auto validate(const Model &obj) -> bool {
        return std::apply(
            [&](auto... rules) -> bool {
                return (decltype(rules)::check(obj) && ...);
            },
            typename Derived::rules{});
    }

    template <typename Field>
        requires core::concepts::TupleContains<typename Derived::fields, Field>
    [[nodiscard]] static auto validateField(const Model &obj) -> bool {
        return std::apply(
            [&](auto... rules) -> bool {
                return (decltype(rules)::check(obj) && ...);
            },
            typename meta::RulesForField<Field,
                                         typename Derived::rules>::type{});
    }
};
}; // namespace core::schema
