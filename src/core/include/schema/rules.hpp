#pragma once

#include <concepts>

#include <concepts.hpp>
#include <error.hpp>
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
    : std::bool_constant<(concepts::Rule<Rs, Model, Fields...> && ...)> {};
} // namespace impl

template <typename RT, typename Model, typename Fields>
concept RuleTuple = impl::RuleTuple<RT, Model, Fields>::value;

template <typename ModelRule, typename Field>
concept RuleContainsField =
    core::concepts::TupleContains<Field, typename ModelRule::fields>;
} // namespace concepts

template <auto Checker, typename Error, typename... Fields>
    requires core::error::Concept<Error>
struct Rule {
    using fields = std::tuple<Fields...>;

    template <typename Model>
    static constexpr auto check(const Model &obj) -> bool {
        return Checker(obj);
    }

    template <typename Model> static constexpr auto validate(const Model &obj) {
        if (!check(obj)) {
            throw Error{};
        }
    }
};

template <auto Checker, typename Error, typename... Fields>
    requires core::error::Concept<Error>
struct Rule<Checker, Error, std::tuple<Fields...>> {
    using fields = std::tuple<Fields...>;

    template <typename Model>
    static constexpr auto check(const Model &obj) -> bool {
        return Checker(obj);
    }

    template <typename Model> static constexpr auto validate(const Model &obj) {
        if (!check(obj)) {
            throw Error{};
        }
    }
};

template <typename... Rs> using Rules = std::tuple<Rs...>;
} // namespace core::schema::rules
