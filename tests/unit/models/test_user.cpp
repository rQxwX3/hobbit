#include <gtest/gtest.h>

#include <user.hpp>

namespace test::mods {
TEST(UserTest, GetSetName) {
    using hbt::mods::User;

    auto user{User{"alice"}};
    EXPECT_EQ(user.getName(), "alice");

    user.setName("bob");
    EXPECT_EQ(user.getName(), "bob");
}

TEST(UserTest, ToFromJSON) {
    using hbt::mods::User;

    auto original{User{"alice"}};

    auto json = original.toJSON();
    ASSERT_EQ(std::string(json.type_name()), "object");
    EXPECT_EQ(json["name"].get<std::string>(), "alice");

    auto restored{User::fromJSON(json)};
    EXPECT_EQ(restored.getName(), "alice");
}
} // namespace test::mods
