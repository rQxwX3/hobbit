#pragma once

#include <nlohmann/json.hpp>
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
    [[nodiscard]] auto toJSON() const & -> nlohmann::json;

    [[nodiscard]] auto toJSON() && -> nlohmann::json;

    [[nodiscard]] static auto fromJSON(const nlohmann::json &j) -> User;
};
} // namespace hbt::mods
