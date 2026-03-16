#include <nlohmann/json.hpp>

#include <user.hpp>

namespace hbt::mods {
User::User(std::string name) : name_{std::move(name)} {}

[[nodiscard]] auto User::getName() const -> std::string_view { return name_; }

auto User::setName(std::string_view name) -> void { name_ = name; }

[[nodiscard]] auto User::toJSON() const & -> nlohmann::json {
    return {{"name", name_}};
}

[[nodiscard]] auto User::toJSON() && -> nlohmann::json {
    return {{"name", std::move(name_)}};
}

[[nodiscard]] auto User::fromJSON(const nlohmann::json &json) -> User {
    if (!json.contains("name")) {
        throw std::runtime_error("Missing required fields");
    }

    return User{json["name"].get<std::string>()};
}
} // namespace hbt::mods
