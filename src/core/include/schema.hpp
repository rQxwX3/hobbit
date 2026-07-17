#pragma once

#include <concepts>

namespace core::schema {
template <typename Derived, typename Model> struct Base {
    [[nodiscard]] static auto validate(const Model &obj) -> bool {
        return Derived::Fields::validate(obj) && Derived::Rules::validate(obj);
    }
};

template <typename S, typename M>
concept Concept = std::same_as<typename S::Model, M> &&
                  std::is_enum_v<typename S::FieldID> && requires(M obj) {
                      typename S::Fields;
                      typename S::RuleSet;
                      typename S::Rules;

                      { S::validate(obj) } -> std::same_as<bool>;
                  };
}; // namespace core::schema
