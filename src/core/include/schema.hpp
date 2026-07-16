#pragma once

#include <concepts>

namespace core::schema {
template <typename S, typename M>
concept Concept = std::same_as<typename S::Model, M> &&
                  std::is_enum_v<typename S::FieldID> && requires(M obj) {
                      typename S::RuleSet;
                      typename S::Fields;
                      typename S::Rules;

                      { S::validate(obj) } -> std::same_as<bool>;
                  };
}; // namespace core::schema
