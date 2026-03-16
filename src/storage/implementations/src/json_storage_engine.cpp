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
    auto json = nlohmann::json(data_);
    auto file{std::ofstream(filename_)};

    file << json.dump(4);
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

[[nodiscard]] auto StorageEngine::getKeyValuePairs() const -> data_t {
    std::unordered_map<std::string, std::string> pairs;

    for (const auto &[key, value] : data_) {
        pairs[key] = value;
    }

    return pairs;
}

auto StorageEngine::clear() -> void {
    data_.clear();
    save();
}
} // namespace hbt::store::json
