#include "../include/day.hpp"
#include "../include/entry.hpp"
#include <chrono>
#include <vector>

namespace hbt {
using time_point = std::chrono::system_clock::time_point;
using std::chrono::floor, std::chrono::days, std::chrono::weekday;

Day::Day(time_point time, std::vector<Entry> entries)
    : weekday_{weekday{floor<days>(time)}}, entries_{std::move(entries)} {}
} // namespace hbt
