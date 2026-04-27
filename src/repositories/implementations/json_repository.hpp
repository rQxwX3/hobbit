#pragma once

#include <nlohmann/json.hpp>

#include <repository.hpp>
#include <storage.hpp>
#include <uuid.hpp>

#include <concepts>
#include <cstddef>
#include <expected>
#include <memory>
#include <optional>
#include <string>

namespace hbt::repo::json {
template <typename T>
concept JSONSerializable =
    requires(const T &const_t, T &t, T &&rvalue_t, nlohmann::json j) {
        { const_t.toJSON() } -> std::convertible_to<nlohmann::json>;

        { std::move(rvalue_t).toJSON() } -> std::convertible_to<nlohmann::json>;

        { T::fromJSON(j) } -> std::same_as<std::expected<T, typename T::Error>>;
    };

template <JSONSerializable T> class Repository {
  public:
    std::shared_ptr<hbt::store::Storage> storage_;

    enum class Error : uint8_t {
        SerializeEmptyString,
    };

  public:
    [[nodiscard]] static constexpr auto errorMessage(Error error)
        -> std::string {
        switch (error) {
        case Error::SerializeEmptyString:
            return "Repository: can't serialize empty input";
        }
    }

  public:
    Repository(std::shared_ptr<hbt::store::Storage> storage)
        : storage_{std::move(storage)} {}

    virtual ~Repository() = default;

  public:
    Repository(const Repository &) = delete;
    Repository(Repository &&) = delete;

    auto operator=(const Repository &) -> Repository & = delete;
    auto operator=(Repository &&) -> Repository & = delete;

  public:
    [[nodiscard]] auto serialize(const T &data) const -> std::string {
        return data.toJSON().dump();
    }

    [[nodiscard]] auto serialize(T &&data) const -> std::string {
        return std::move(data).toJSON().dump();
    }

    [[nodiscard]] auto deserialize(const std::string &raw) const
        -> std::expected<T, Error> {
        if (raw.empty()) {
            return std::unexpected(Error::SerializeEmptyString);
        }

        return T::fromJSON(nlohmann::json::parse(raw));
    }

  public:
    [[nodiscard]] auto getAll() const -> std::vector<T> {
        std::vector<T> result;

        for (const auto &[key, value] : storage_->getKeyValuePairs()) {
            if (auto obj{deserialize(value)}) {
                result.push_back(obj);
            }
        }

        return result;
    }

    auto clear() -> void { storage_->clear(); }
};

template <JSONSerializable T>
class SingleItemRepository : public hbt::repo::SingleItemRepository<T> {
  private:
    hbt::repo::json::Repository<T> base;

  public:
    explicit SingleItemRepository(std::shared_ptr<hbt::store::Storage> storage)
        : base{std::move(storage)} {}

  public:
    auto save(const T &data) -> void override {
        base.storage_->write("1", base.serialize(data));
    }

    auto save(T &&data) -> void override {
        base.storage_->write("1", base.serialize(std::move(data)));
    }

    auto update(const T &data) -> void override { save(data); }

    [[nodiscard]] auto load() const -> std::optional<T> override {
        auto value{base.storage_->read("1")};

        if (!value) {
            return std::nullopt;
        }

        return base.deserialize(*value);
    }

    auto remove() -> void override { base.storage_->remove("1"); }

    [[nodiscard]] auto exists() const -> bool override {
        return base.storage_->exists("1");
    }

  public:
    [[nodiscard]] auto getAll() const -> std::vector<T> override {
        std::vector<T> result;

        for (const auto &item : base.storage_->getKeyValuePairs()) {
            if (auto obj{base.deserialize(item.second)}) {
                result.push_back(obj.value());
            }
        }

        return result;
    }

    auto clear() -> void override { base.storage_->clear(); }
};

template <typename IDT>
concept IDCompatible = std::same_as<IDT, core::uuid::uuid_t>;

template <JSONSerializable T, IDCompatible IDT = core::uuid::uuid_t>
class MultiItemRepository : public hbt::repo::MultiItemRepository<T, IDT> {
  private:
    hbt::repo::json::Repository<T> base_;

  public:
    explicit MultiItemRepository(std::shared_ptr<hbt::store::Storage> storage)
        : base_{std::move(storage)} {}

  public:
    auto save(const T &data) -> IDT override {
        auto uuid{core::uuid::generateUUID()};

        base_.storage_->write(uuid, base_.serialize(data));

        return uuid;
    }

    auto save(T &&data) -> IDT override {
        auto uuid{core::uuid::generateUUID()};

        base_.storage_->write(uuid, base_.serialize(std::move(data)));

        return uuid;
    }

    auto update(const IDT &id, const T &data) -> void override {
        if (!exists(id)) {
            return;
        }

        base_.storage_->write(id, base_.serialize(data));
    }

    [[nodiscard]] auto load(const IDT &id) const -> std::optional<T> override {
        auto value{base_.storage_->read(id)};

        if (!value) {
            return std::nullopt;
        }

        return base_.deserialize(*value);
    }

    auto remove(const IDT &id) -> void override { base_.storage_->remove(id); }

    [[nodiscard]] auto exists(const IDT &id) const -> bool override {
        return base_.storage_->exists(id);
    }

  public:
    [[nodiscard]] auto getAll() const -> std::vector<T> override {
        std::vector<T> result;

        for (const auto &item : base_.storage_->getKeyValuePairs()) {
            if (auto obj{base_.deserialize(item.second)}) {
                result.push_back(obj.value());
            }
        }

        return result;
    }

    auto clear() -> void override { base_.storage_->clear(); }
};
} // namespace hbt::repo::json
