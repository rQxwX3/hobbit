#pragma once

#include "day_part.hpp"
#include <chrono>

namespace hbt {
struct Occurence {
    std::chrono::weekday weekday;
    DayPart daypart;
};
} // namespace hbt
