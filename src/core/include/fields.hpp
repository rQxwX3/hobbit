#pragma once

#include <fixed_string.hpp>
#include <validation.hpp>

namespace core::schema {
template <auto fieldID, core::FixedString string, auto Getter, typename T,
          typename... Rules>
struct Field {
    static constexpr auto id{fieldID};
    static constexpr auto name{string};
    static constexpr auto getter{Getter};

    using type = T;

    using rules = core::validation::FieldRules<Rules...>;
};

template <typename ID_t, typename... Fs> struct Fields {
    using tuple = std::tuple<Fs...>;

    template <ID_t id>
    using field = std::tuple_element_t<static_cast<std::size_t>(id), tuple>;

    template <ID_t id> using name = typename field<id>::name;

    template <ID_t id> using type = typename field<id>::type;

    template <ID_t id> using rules = typename field<id>::rules;

    template <typename Model>
    [[nodiscard]] static constexpr auto validate(const Model &obj) -> bool {
        return (Fs::rules::validate((obj.*Fs::getter)()) && ...);
    }
};
}; // namespace core::schema
