#pragma once

#include <algorithm>
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

template <typename... Rs> struct FieldRules {
    template <typename T> static auto validate(T const &value) -> bool {
        return (Rs::check(value) && ...);
    }
};

template <typename Model, typename... Rs> struct ModelRules {
    static auto validate(Model const &model) -> bool {
        return (Rs::check(model) && ...);
    }
};

template <typename Rule> struct Each {
    template <typename Container> static bool check(const Container &c) {
        return std::ranges::all_of(
            c, [](const auto &value) -> auto { return Rule::check(value); });
    }
};

template <auto Max> struct Less {
    template <typename T>
    [[nodiscard]] static auto check(const T &value) -> bool {
        return value < Max;
    }
};

template <auto Min, auto Max> struct Range {
    template <typename T>
    [[nodiscard]] static auto check(const T &value) -> bool {
        return Min <= value && value <= Max;
    }
};

template <auto Min, auto Max> struct EnumRange {
    template <typename E>
        requires std::is_enum_v<E>

    static constexpr auto check(E value) -> bool {
        using U = std::underlying_type_t<E>;
        auto v = static_cast<U>(value);
        return v >= static_cast<U>(Min) && v <= static_cast<U>(Max);
    }
};

static_assert(TypeRule<Range<0, 10>, int>);
}; // namespace core::validation
