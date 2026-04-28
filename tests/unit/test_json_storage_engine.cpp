// #include <filesystem>
// #include <fstream>
//
// #include <gtest/gtest.h>
// #include <nlohmann/json.hpp>
//
// #include <json_storage.hpp>
//
// namespace test::store::json {
// class StorageEngineTest : public ::testing::Test {
//   protected:
//     const std::string test_filename = "test_storage.json";
//
//     auto SetUp() -> void override { cleanup(); }
//
//     auto TearDown() -> void override { cleanup(); }
//
//     auto cleanup() -> void {
//         if (std::filesystem::exists(test_filename)) {
//             std::filesystem::remove(test_filename);
//         }
//     }
//
//     auto createTestFile(const nlohmann::json &data) -> void {
//         std::ofstream file(test_filename);
//
//         file << data.dump(2);
//         file.close();
//     }
//
//     auto readFile() -> nlohmann::json {
//         std::ifstream file(test_filename);
//
//         return nlohmann::json::parse(file);
//     }
// };
//
// TEST_F(StorageEngineTest, ConstructorDoesNotOpenFile) {
//     EXPECT_FALSE(std::filesystem::exists(test_filename));
//
//     auto storage{hbt::store::json::Storage(test_filename)};
//     EXPECT_EQ(storage.getCount(), 0);
//     EXPECT_FALSE(std::filesystem::exists(test_filename));
// }
//
// TEST_F(StorageEngineTest, ConstructorHandlesEmptyJSON) {
//     createTestFile(nlohmann::json::object());
//     auto storage{hbt::store::json::Storage(test_filename)};
//
//     EXPECT_EQ(storage.getCount(), 0);
// }
//
// TEST_F(StorageEngineTest, WriteCreatesFileAndSavesData) {
//     auto storage{hbt::store::json::Storage(test_filename)};
//     storage.write("key", "value");
//     ASSERT_TRUE(std::filesystem::exists(test_filename));
//
//     auto json = readFile();
//     ASSERT_EQ(std::string(json.type_name()), "object");
//     EXPECT_EQ(json["key"], "value");
// }
//
// TEST_F(StorageEngineTest, WriteUpdatesExsitingKey) {
//     auto storage{hbt::store::json::Storage(test_filename)};
//     storage.write("key", "value1");
//     storage.write("key", "value2");
//
//     auto json = readFile();
//     EXPECT_EQ(json["key"], "value2");
//     EXPECT_EQ(json.size(), 1);
// }
//
// TEST_F(StorageEngineTest, WriteAddsMultipleKeys) {
//     auto storage{hbt::store::json::Storage(test_filename)};
//
//     storage.write("key1", "value1");
//     storage.write("key2", "value2");
//     storage.write("key3", "value3");
//
//     auto json = readFile();
//     EXPECT_EQ(json.size(), 3);
//     EXPECT_EQ(json["key1"], "value1");
//     EXPECT_EQ(json["key2"], "value2");
//     EXPECT_EQ(json["key3"], "value3");
// }
//
// TEST_F(StorageEngineTest, WritePreservesExistingData) {
//     {
//         auto storage{hbt::store::json::Storage(test_filename)};
//         storage.write("key1", "value1");
//     }
//
//     {
//         auto storage{hbt::store::json::Storage(test_filename)};
//         storage.write("key2", "value2");
//     }
//
//     auto json = readFile();
//     EXPECT_EQ(json.size(), 2);
//     EXPECT_EQ(json["key1"], "value1");
//     EXPECT_EQ(json["key2"], "value2");
// }
//
// TEST_F(StorageEngineTest, ReadReturnsValueForExistingKey) {
//     auto storage{hbt::store::json::Storage(test_filename)};
//     storage.write("key", "value");
//
//     auto result{storage.read("key")};
//     ASSERT_TRUE(result.has_value());
//     EXPECT_EQ(result.value(), "value");
// }
//
// TEST_F(StorageEngineTest, ReadReturnsNulloptForMissingKey) {
//     auto storage{hbt::store::json::Storage(test_filename)};
//     storage.write("key", "value");
//
//     auto result{storage.read("nonexistent")};
//     EXPECT_FALSE(result.has_value());
// }
//
// TEST_F(StorageEngineTest, ReadWorksWithEmptyStringKey) {
//     auto storage{hbt::store::json::Storage(test_filename)};
//     storage.write("", "value");
//
//     auto result{storage.read("")};
//     ASSERT_TRUE(result.has_value());
//     EXPECT_EQ(result.value(), "value");
// }
//
// TEST_F(StorageEngineTest, ReadWorksWithEmptyStringValue) {
//     auto storage{hbt::store::json::Storage(test_filename)};
//     storage.write("key", "");
//
//     auto result{storage.read("key")};
//     ASSERT_TRUE(result.has_value());
//     EXPECT_EQ(result.value(), "");
// }
//
// TEST_F(StorageEngineTest, RemoveExistingKey) {
//     auto storage{hbt::store::json::Storage(test_filename)};
//     storage.write("key1", "value1");
//     storage.write("key2", "value2");
//
//     storage.remove("key1");
//     EXPECT_EQ(storage.getCount(), 1);
//     EXPECT_FALSE(storage.exists("key1"));
//     EXPECT_TRUE(storage.exists("key2"));
//
//     auto json = readFile();
//     EXPECT_EQ(json.size(), 1);
//     EXPECT_FALSE(json.contains("key1"));
//     EXPECT_EQ(json["key2"], "value2");
// }
//
// TEST_F(StorageEngineTest, RemoveNonExistingKeyDoesNothing) {
//     auto storage{hbt::store::json::Storage(test_filename)};
//     storage.write("key1", "value1");
//
//     storage.remove("nonexistent");
//
//     EXPECT_EQ(storage.getCount(), 1);
//     EXPECT_TRUE(storage.exists("key1"));
//
//     auto json = readFile();
//     EXPECT_EQ(json.size(), 1);
//     EXPECT_EQ(json["key1"], "value1");
// }
//
// TEST_F(StorageEngineTest, RemoveLastKeyCreatesEmptyJsonObject) {
//     auto storage{hbt::store::json::Storage(test_filename)};
//     storage.write("key", "value");
//
//     storage.remove("key");
//     EXPECT_EQ(storage.getCount(), 0);
//
//     auto json = readFile();
//     EXPECT_TRUE(json.empty());
// }
//
// TEST_F(StorageEngineTest, ExistsReturnsTrueForExistingKey) {
//     auto storage{hbt::store::json::Storage(test_filename)};
//     storage.write("key", "value");
//
//     EXPECT_TRUE(storage.exists("key"));
// }
//
// TEST_F(StorageEngineTest, ExistsReturnsFalseForNonExistingKey) {
//     auto storage{hbt::store::json::Storage(test_filename)};
//     storage.write("key", "value");
//
//     EXPECT_FALSE(storage.exists("nonexistent"));
// }
//
// TEST_F(StorageEngineTest, ExistsReturnsFalseForRemovedKey) {
//     auto storage{hbt::store::json::Storage(test_filename)};
//     storage.write("key", "value");
//
//     storage.remove("key");
//     EXPECT_FALSE(storage.exists("key"));
// }
//
// TEST_F(StorageEngineTest, GetCountReturnsCorrectNumberOfKeys) {
//     auto storage{hbt::store::json::Storage(test_filename)};
//
//     EXPECT_EQ(storage.getCount(), 0);
//
//     storage.write("key1", "value1");
//     EXPECT_EQ(storage.getCount(), 1);
//
//     storage.write("key2", "value2");
//     EXPECT_EQ(storage.getCount(), 2);
//
//     storage.remove("key1");
//     EXPECT_EQ(storage.getCount(), 1);
//
//     storage.clear();
//     EXPECT_EQ(storage.getCount(), 0);
// }
//
// TEST_F(StorageEngineTest, GetKeyValuePairsReturnsAllPairs) {
//     auto storage{hbt::store::json::Storage(test_filename)};
//
//     storage.write("key1", "value1");
//     storage.write("key2", "value2");
//     storage.write("key3", "value3");
//
//     auto data{storage.getKeyValuePairs()};
//     EXPECT_EQ(data.size(), 3);
//     EXPECT_EQ(data["key1"], "value1");
//     EXPECT_EQ(data["key2"], "value2");
//     EXPECT_EQ(data["key3"], "value3");
// }
//
// TEST_F(StorageEngineTest, GetKeyValuePairsReturnsEmptyForEmptyStorage) {
//     auto storage{hbt::store::json::Storage(test_filename)};
//
//     auto data{storage.getKeyValuePairs()};
//     EXPECT_TRUE(data.empty());
// }
//
// TEST_F(StorageEngineTest, ClearRemovesAllData) {
//     auto storage{hbt::store::json::Storage(test_filename)};
//     storage.write("key1", "value1");
//     storage.write("key2", "value2");
//
//     storage.clear();
//
//     EXPECT_EQ(storage.getCount(), 0);
//     EXPECT_FALSE(storage.exists("key1"));
//     EXPECT_FALSE(storage.exists("key2"));
//
//     EXPECT_TRUE(readFile().empty());
// }
//
// TEST_F(StorageEngineTest, ClearOnEmptyStorageDoesNothing) {
//     auto storage{hbt::store::json::Storage(test_filename)};
//
//     storage.clear();
//     EXPECT_EQ(storage.getCount(), 0);
//
//     if (std::filesystem::exists(test_filename)) {
//         EXPECT_TRUE(readFile().empty());
//     }
// }
//
// TEST_F(StorageEngineTest, HandlesSpecialCharactersInKeys) {
//     auto storage{hbt::store::json::Storage(test_filename)};
//
//     std::vector<std::string> specialKeys{
//         {"key with spaces", "key!@#$%^&*()", "key.with.dots",
//         "key-with-dashes",
//          "key_with_underscores", "123numeric", "🚀emoji", "привет",
//          "안녕하세요", "🌍🌎🌏"}};
//
//     for (const auto &key : specialKeys) {
//         storage.write(key, "value");
//     }
//
//     EXPECT_EQ(storage.getCount(), specialKeys.size());
//
//     for (const auto &key : specialKeys) {
//         ASSERT_TRUE(storage.exists(key));
//         EXPECT_EQ(storage.read(key), "value");
//     }
//
//     EXPECT_EQ(readFile().size(), specialKeys.size());
// }
//
// TEST_F(StorageEngineTest, HandlesSpecialCharactersInValues) {
//     auto storage{hbt::store::json::Storage(test_filename)};
//
//     std::string specialValue{
//         "Value with spaces!@#$%^&*() and emojis 🚀 and unicode 你好"};
//     storage.write("key", specialValue);
//
//     EXPECT_EQ(storage.read("key"), specialValue);
//     EXPECT_EQ(readFile()["key"], specialValue);
// }
//
// TEST_F(StorageEngineTest, HandlesLargeValues) {
//     constexpr size_t largeInteger{10000};
//
//     auto storage{hbt::store::json::Storage(test_filename)};
//     std::string largeValue(largeInteger, 'a');
//
//     storage.write("key", largeValue);
//     EXPECT_EQ(storage.read("key"), largeValue);
//     EXPECT_EQ(readFile()["key"], largeValue);
// }
//
// TEST_F(StorageEngineTest, HandlesLargeNumberOfKeys) {
//     constexpr size_t largeInteger{1000};
//
//     auto storage{hbt::store::json::Storage(test_filename)};
//
//     for (int i = 0; i < largeInteger; ++i) {
//         storage.write("key" + std::to_string(i), "value" +
//         std::to_string(i));
//     }
//
//     EXPECT_EQ(storage.getCount(), largeInteger);
//
//     for (int i = 0; i < largeInteger; ++i) {
//         EXPECT_EQ(storage.read("key" + std::to_string(i)),
//                   "value" + std::to_string(i));
//     }
// }
//
// TEST_F(StorageEngineTest, SaveUsesIndentedFormat) {
//     auto storage{hbt::store::json::Storage(test_filename)};
//     storage.write("key", "value");
//
//     std::ifstream file(test_filename);
//     std::string content((std::istreambuf_iterator<char>(file)),
//                         std::istreambuf_iterator<char>());
//
//     EXPECT_TRUE(content.find("    \"key\"") != std::string::npos);
// }
//
// TEST_F(StorageEngineTest, LoadHandlesMinifiedJson) {
//     std::ofstream file(test_filename);
//     file << R"({"key":"value"})";
//     file.close();
//
//     auto storage{hbt::store::json::Storage(test_filename)};
//     EXPECT_EQ(storage.getCount(), 1);
//     EXPECT_EQ(storage.read("key"), "value");
// }
// } // namespace test::store::json
