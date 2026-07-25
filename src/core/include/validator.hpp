#pragma once

#include <concepts>

#include <model.hpp>

namespace core::validator {
template <typename V, typename M>
concept Concept = core::model::Concept<M> && requires(M model) {
    { V::validate(model) } -> std::same_as<bool>;
    { V::validateAndReturn(model) } -> std::same_as<M>;

    { V::template validate<M::Field::count_>(model) } -> std::same_as<bool>;
    {
        V::template validateAndReturn<M::Field::count_>(model)
    } -> std::same_as<typename M::template FieldType<M::Field::count_>::type>;
};
}; // namespace core::validator
