#pragma once

#include <expected>

#include <nlohmann/json.hpp>

#include <error.hpp>

namespace core::json {
template <typename Derived> class Base {
  public:
    [[nodiscard]] static auto containsAllFields(const nlohmann::json &json)
        -> bool {
        return std::ranges::all_of(Derived::fields,
                                   [&json](const auto &field) -> bool {
                                       return json.contains(field);
                                   });
    }

    [[nodiscard]] static auto getFieldName(auto field) -> std::string_view {
        return Derived::fields[static_cast<size_t>(field)];
    }
};

template <typename T, typename Serializable>
concept Concept =
    std::is_enum_v<typename T::Field> && err::Concept<typename T::Error> &&
    std::same_as<std::underlying_type_t<typename T::Field>, std::size_t> &&
    requires(const Serializable &serializable, const nlohmann::json &json) {
        // { T::fields } -> std::convertible_to<std::vector<std::string_view>
        // &>;
        { T::encode(serializable) } -> std::same_as<nlohmann::json>;
        {
            T::decode(json)
        } -> std::same_as<std::expected<Serializable, typename T::Error::Code>>;
    };
}; // namespace core::json
