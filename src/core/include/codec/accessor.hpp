#pragma once

#include <concepts>

namespace core::codec::accessor {
template <typename A, typename Data, typename Type>
concept Concept = requires(A accessor, Data data) {
    { accessor(data) } -> std::same_as<Type>;
};
}; // namespace core::codec::accessor
