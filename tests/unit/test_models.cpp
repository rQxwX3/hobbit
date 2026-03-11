#include <entry.hpp>
#include <user.hpp>

#include <gtest/gtest.h>

TEST(UserTest, GetSetName) {
    auto user{hbt::mods::User{"alice"}};
    EXPECT_EQ(user.getName(), "alice");

    user.setName("bob");
    EXPECT_EQ(user.getName(), "bob");
}

// TEST(UserTest, ToJSONFromJSON) {
//     auto original{hbt::mods::User{"alice"}};
//
//     auto json{original.toJSON()};
//     auto restored{hbt::mods::User::fromJSON()}
// }

// TEST(EntryTest, )
