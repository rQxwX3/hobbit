#pragma once

#include <fixed_string.hpp>
#include <validation.hpp>

namespace core::schema {
template <auto fieldID, core::FixedString string, typename T, typename... Rules>
struct Field {
    static constexpr auto id{fieldID};
    static constexpr auto name{string};

    using type = T;

    using rules = core::validation::FieldRules<Rules...>;
};

template <typename... Fs> struct Fields {
    using tuple = std::tuple<Fs...>;

    template <auto ID>
    using field = std::tuple_element_t<static_cast<std::size_t>(ID), tuple>;

    template <auto ID> using type = typename field<ID>::type;

    template <auto ID> using rules = typename field<ID>::rules;

    template <auto ID> static constexpr auto name = field<ID>::name;
};

template <typename S, typename M>
concept Concept = std::is_enum_v<typename S::FieldID> && requires(M obj) {
    typename S::Model;

    typename S::RuleSet;
    typename S::Fields;
    typename S::Rules;

    { S::validate(obj) } -> std::same_as<bool>;
};
}; // namespace core::schema
