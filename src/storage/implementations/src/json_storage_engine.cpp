#include <json_storage_engine.hpp>
#include <storage_engine.hpp>

#include <nlohmann/json.hpp>

#include <fstream>
#include <string>

namespace hbt::store::json {
StorageEngine::StorageEngine(std::string filename)
    : filename_{std::move(filename)} {
    load();
}

auto StorageEngine::load() -> void {
    auto file{std::ifstream(filename_)};

    if (!file) {
        return;
    }

    nlohmann::json j;
    file >> j;

    data_ = j.get<StorageEngine::data_t>();
}

auto StorageEngine::save() -> void {
    auto j{nlohmann::json(data_)};
    auto file{std::ofstream(filename_)};

    file << j.dump(4);
}

auto StorageEngine::write(const std::string &key, const std::string &value)
    -> void {
    data_[key] = value;

    save();
}

[[nodiscard]] auto StorageEngine::read(const std::string &key) const
    -> std::optional<std::string> {
    auto it{data_.find(key)};

    if (it == data_.end()) {
        return std::nullopt;
    }

    return it->second;
}

auto StorageEngine::remove(const std::string &key) -> void {
    data_.erase(key);
    save();
}

[[nodiscard]] auto StorageEngine::exists(const std::string &key) const -> bool {
    return data_.contains(key);
}

[[nodiscard]] auto StorageEngine::getCount() const -> size_t {
    return data_.size();
}

[[nodiscard]] auto StorageEngine::getAll() const -> std::vector<std::string> {
    std::vector<std::string> values;

    for (const auto &[key, value] : data_) {
        values.push_back(value);
    }

    return values;
}

[[nodiscard]] auto StorageEngine::getKeys() const -> std::vector<std::string> {
    std::vector<std::string> keys;

    for (const auto &[key, value] : data_) {
        keys.push_back(key);
    }

    return keys;
}

auto StorageEngine::clear() -> void {
    data_.clear();
    save();
}
} // namespace hbt::store::json
