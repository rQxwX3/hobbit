#pragma once

#include <concepts>

namespace core::codec::processor {
template <typename P, typename From, typename To>
concept Concept = requires(P processor, From from) {
    { processor(from) } -> std::same_as<To>;
};
}; // namespace core::codec::processor
