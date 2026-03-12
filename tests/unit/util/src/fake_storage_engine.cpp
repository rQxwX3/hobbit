#include <storage_engine.hpp>

#include <fake_storage_engine.hpp>

auto FakeStorageEngine::write(const std::string &key, const std::string &value)
    -> void {
    data_[key] = value;
}

[[nodiscard]] auto FakeStorageEngine::read(const std::string &key) const
    -> std::optional<std::string> {
    auto it{data_.find(key)};

    if (it == data_.end()) {
        return std::nullopt;
    }

    return it->second;
}

auto FakeStorageEngine::remove(const std::string &key) -> void {
    data_.erase(key);
}

[[nodiscard]] auto FakeStorageEngine::exists(const std::string &key) const
    -> bool {
    return data_.contains(key);
}

[[nodiscard]] auto FakeStorageEngine::getCount() const -> size_t {
    return data_.size();
}

[[nodiscard]] auto FakeStorageEngine::getKeyValuePairs() const
    -> std::vector<std::pair<std::string, std::string>> {
    auto result{std::vector<std::pair<std::string, std::string>>()};
    result.reserve(data_.size());

    for (const auto &[key, value] : data_) {
        result.emplace_back(key, value);
    }

    return result;
}

auto FakeStorageEngine::clear() -> void { data_.clear(); }
