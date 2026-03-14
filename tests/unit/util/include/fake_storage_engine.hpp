#include <storage_engine.hpp>

#include <unordered_map>

class FakeStorageEngine : public hbt::store::StorageEngine {
  private:
    std::unordered_map<std::string, std::string> data_;

  public:
    auto write(const std::string &key, const std::string &value)
        -> void override;

    [[nodiscard]] auto read(const std::string &key) const
        -> std::optional<std::string> override;

    auto remove(const std::string &key) -> void override;

    [[nodiscard]] auto exists(const std::string &key) const -> bool override;

  public:
    [[nodiscard]] auto getCount() const -> size_t override;

    [[nodiscard]] auto getKeyValuePairs() const
        -> std::unordered_map<std::string, std::string> override;

    auto clear() -> void override;

  public:
    ~FakeStorageEngine() override = default;
};
