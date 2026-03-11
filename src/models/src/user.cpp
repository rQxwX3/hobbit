#include <user.hpp>

namespace hbt::mods {
User::User(std::string name) : name_{std::move(name)} {}

[[nodiscard]] auto User::getName() const -> std::string_view { return name_; }

auto User::setName(std::string_view name) -> void { name_ = name; }
} // namespace hbt::mods
