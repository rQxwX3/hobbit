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

template <typename ID_t, typename... Fs> struct Fields {
    using tuple = std::tuple<Fs...>;

  private:
    template <ID_t id>
    using field = std::tuple_element_t<static_cast<std::size_t>(id), tuple>;

  public:
    template <ID_t id> using type = typename field<id>::type;
    template <ID_t id> using rules = typename field<id>::rules;
    template <ID_t id> static constexpr auto name{field<id>::name};
};
}; // namespace core::schema
