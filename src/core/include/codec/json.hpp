#pragma once

#include <nlohmann/json.hpp>

#include <codec/codec.hpp>
#include <codec/fields.hpp>
#include <error.hpp>
#include <schema/schema.hpp>

namespace core::codec::json {
namespace concepts {
template <typename T, typename Model>
concept Codec = requires(nlohmann::json json) {
    requires codec::concepts::Codec<T, Model, nlohmann::json>;

    { T::containsAllFields(json) } -> std::same_as<bool>;
};
} // namespace concepts

template <typename FT> struct Codec : codec::Codec<FT> {
    using typename codec::Codec<FT>::fields;

  private:
    template <typename Tuple> struct ContainsAllFields;

    template <typename... Fields>
    struct ContainsAllFields<std::tuple<Fields...>> {
        static auto check(const nlohmann::json &json) -> bool {
            return (json.contains(Fields::name) && ...);
        }
    };

  public:
    static auto containsAllFields(const nlohmann::json &json) -> bool {
        return ContainsAllFields<fields>::check(json);
    }
};
}; // namespace core::codec::json
