#include <interval.hpp>

namespace hbt::mods {
[[nodiscard]] auto Interval::toMinutes() -> std::chrono::minutes {
    return std::visit(
        [](auto arg) -> auto {
            return std::chrono::duration_cast<std::chrono::minutes>(arg);
        },
        duration_);
}

[[nodiscard]] auto Interval::toHours() -> std::chrono::hours {
    return std::visit(
        [](auto arg) -> auto {
            return std::chrono::duration_cast<std::chrono::hours>(arg);
        },
        duration_);
}

[[nodiscard]] auto Interval::toDays() -> std::chrono::days {
    return std::visit(
        [](auto arg) -> auto {
            return std::chrono::duration_cast<std::chrono::days>(arg);
        },
        duration_);
}

[[nodiscard]] auto Interval::toWeeks() -> std::chrono::weeks {
    return std::visit(
        [](auto arg) -> auto {
            return std::chrono::duration_cast<std::chrono::weeks>(arg);
        },
        duration_);
}

[[nodiscard]] auto Interval::toMonths() -> std::chrono::months {
    return std::visit(
        [](auto arg) -> auto {
            return std::chrono::duration_cast<std::chrono::months>(arg);
        },
        duration_);
}

[[nodiscard]] auto Interval::toYears() -> std::chrono::years {
    return std::visit(
        [](auto arg) -> auto {
            return std::chrono::duration_cast<std::chrono::years>(arg);
        },
        duration_);
}
} // namespace hbt::mods
