#pragma once

#include <string>

namespace core::err {
template <typename Derived> class Base {
  public:
    [[nodiscard]] static auto generateMessage(std::string_view message)
        -> std::string {
        return std::string(Derived::className) + ": " + std::string(message);
    }
};

template <typename T>
concept Concept = std::is_enum_v<typename T::Code> &&
                  requires(typename T::Code code, std::string_view message) {
                      typename T::Code;
                      { T::className } -> std::convertible_to<std::string_view>;
                      {
                          T::generateMessage(message)
                      } -> std::same_as<std::string>;
                      { T::getMessage(code) } -> std::same_as<std::string>;
                  };
}; // namespace core::err
