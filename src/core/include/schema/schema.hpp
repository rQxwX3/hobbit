#pragma once

#include <error.hpp>
#include <schema/fields.hpp>
#include <schema/rules.hpp>

namespace core::schema {
namespace meta {
template <typename Field, typename Rules> struct RulesForField;

/* declaration */
template <typename... Tuples> struct Join;

/* base case */
template <> struct Join<> {
    using type = std::tuple<>;
};

/* recursive case */
template <typename... Ts, typename... Us, typename... Rest>
struct Join<std::tuple<Ts...>, std::tuple<Us...>, Rest...> {
    using type = typename Join<std::tuple<Ts..., Us...>, Rest...>::type;
};

template <typename Field, typename... Rules>
struct RulesForField<Field, std::tuple<Rules...>> {
    template <typename Rule>
    using Match =
        std::conditional_t<rules::concepts::RuleContainsField<Rule, Field>,
                           std::tuple<Rule>, std::tuple<>>;

    using type = typename Join<Match<Rules>...>::type;
};
} // namespace meta

template <typename Model, typename Fields, typename Rules>
    requires fields::concepts::FieldTuple<Fields, Model> &&
             schema::rules::concepts::RuleTuple<Rules, Model, Fields>
struct Schema {
    using model = Model;
    using fields = Fields;
    using rules = Rules;

    [[nodiscard]] static auto checkAllRules(const model &obj) -> bool {
        return std::apply(
            [&](auto... rules) -> bool {
                return (decltype(rules)::template check<model>(obj) && ...);
            },
            rules{});
    }

    template <typename Field>
        requires core::concepts::TupleContains<Field, fields>
    [[nodiscard]] static auto checkAffectedRules(const model &obj) -> bool {
        return std::apply(
            [&](auto... rules) -> bool {
                return (decltype(rules)::template check<model>(obj) && ...);
            },
            typename meta::RulesForField<Field, rules>::type{});
    }

    [[nodiscard]] static auto validateAllRules(const model &obj) -> bool {
        return std::apply(
            [&](auto... rules) -> void {
                (decltype(rules)::template validate<model>(obj) && ...);
            },
            rules{});
    }

    template <typename Field>
        requires core::concepts::TupleContains<Field, fields>
    [[nodiscard]] static auto validateAffectedRules(const model &obj) -> bool {
        return std::apply(
            [&](auto... rules) -> void {
                (decltype(rules)::template validate<model>(obj) && ...);
            },
            typename meta::RulesForField<Field, rules>::type{});
    }
};
}; // namespace core::schema
