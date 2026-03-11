#pragma once

#include <string>

namespace hbt::mods {
class User {
  private:
    std::string name_;

  public:
    User(std::string name);

    User(const User &) = delete;
    User(User &&) = delete;

    auto operator=(const User &) -> User & = delete;
    auto operator=(User &&) -> User & = delete;

  public:
    [[nodiscard]] auto getName() const -> std::string_view;

    auto setName(std::string_view) -> void;

  public:
    // [[nodiscard]] auto toJSON() const ->
};
} // namespace hbt::mods
