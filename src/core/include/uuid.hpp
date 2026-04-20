#pragma once

#include <uuid.h>

#include <random>
#include <string>

namespace hbt::core::uuid {
[[nodiscard]] inline auto generateUUID() -> std::string {
    static std::random_device rd;
    static std::mt19937 gen{rd()};
    static uuids::uuid_random_generator uuidGen{gen};

    return uuids::to_string(uuidGen());
}
}; // namespace hbt::core::uuid
