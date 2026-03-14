#include <fake_model.hpp>

#include <nlohmann/json.hpp>
#include <utility>

namespace test::util {
FakeModel::FakeModel(std::string field) : field{std::move(field)} {}

[[nodiscard]] auto FakeModel::toJSON() const -> nlohmann::json {
    return {{"field", field}};
}

[[nodiscard]] auto FakeModel::fromJSON(const nlohmann::json &j) -> FakeModel {
    if (!j.is_object() || !j.contains("field") || !j["field"].is_string()) {
        throw std::runtime_error("Invalid JSON for FakeModel");
    }

    return FakeModel{j["field"]};
}
} // namespace test::util
