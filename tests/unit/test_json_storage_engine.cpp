#include <filesystem>
#include <fstream>

#include <gtest/gtest.h>
#include <nlohmann/json.hpp>

#include <json_storage_engine.hpp>

class JsonStorageEngineTest : public ::testing::Test {
  protected:
    const std::string test_filename = "test_storage.json";

    auto SetUp() -> void override { cleanup(); }

    auto TearDown() -> void override { cleanup(); }

    auto cleanup() -> void {
        if (std::filesystem::exists(test_filename)) {
            std::filesystem::remove(test_filename);
        }
    }

    auto createTestFile(const nlohmann::json &data) -> void {
        std::ofstream file(test_filename);

        file << data.dump(2);
        file.close();
    }

    auto readFile() -> nlohmann::json {
        std::ifstream file(test_filename);

        return nlohmann::json::parse(file);
    }
};
