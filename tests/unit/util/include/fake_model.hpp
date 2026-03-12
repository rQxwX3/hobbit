#include <nlohmann/json.hpp>

#include <string>

class FakeModel {
  public:
    std::string field;

  public:
    FakeModel(std::string field);

  public:
    [[nodiscard]] auto toJSON() const -> nlohmann::json;

    [[nodiscard]] static auto fromJSON(const nlohmann::json &j) -> FakeModel;
};
