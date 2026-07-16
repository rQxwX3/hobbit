#pragma once

#include <concepts>

namespace core::validation {
template <typename R, typename T>
concept TypeRule = requires(T const &value) {
    { R::check(value) } -> std::same_as<bool>;
};

template <typename R, typename M>
concept ModelRule = requires(M const &object) {
    { R::check(object) } -> std::same_as<bool>;
};

template <typename... Rs> struct FieldRules;

template <typename Model, typename... Rs> struct ModelRules {
    template <typename F> static constexpr auto forEach(F &&f) -> void {
        (f.template operator()<Rs>(), ...);
    }

    static auto validate(Model const &model) -> bool {
        return (Rs::check(model) && ...);
    }
};

template <auto Min, auto Max> struct Range {
    template <typename T>
    [[nodiscard]] static auto check(const T &value) -> bool {
        return Min <= value && value <= Max;
    }
};

static_assert(TypeRule<Range<0, 10>, int>);
}; // namespace core::validation
