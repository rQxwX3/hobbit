#include <json_storage.hpp>
#include <storage.hpp>

#include <nlohmann/json.hpp>

#include <fstream>
#include <string>

namespace hbt::store::json {
Storage::Storage(std::string filename) : filename_{std::move(filename)} {
    load();
}

auto Storage::load() -> void {
    auto file{std::ifstream(filename_)};

    if (!file) {
        return;
        // TODO: error
    }

    nlohmann::json j;
    file >> j;

    pairs_ = j.get<pairs_t>();
}

auto Storage::save() -> void {
    auto json = nlohmann::json(pairs_);
    auto file{std::ofstream(filename_)};

    file << json.dump(4);
}

auto Storage::write(const key_t &key, const value_t &value) -> void {
    pairs_[key] = value;

    save();
}

[[nodiscard]] auto Storage::read(const key_t &key) const
    -> std::optional<std::string> {
    auto it{pairs_.find(key)};

    if (it == pairs_.end()) {
        // TODO error
        return std::nullopt;
    }

    return it->second;
}

auto Storage::remove(const key_t &key) -> void {
    pairs_.erase(key);
    save();
}

[[nodiscard]] auto Storage::exists(const key_t &key) const -> bool {
    return pairs_.contains(key);
}

[[nodiscard]] auto Storage::getKeyValuePairs() const -> pairs_t {
    return pairs_;
}

auto Storage::clear() -> void {
    pairs_.clear();
    save();
}
} // namespace hbt::store::json
