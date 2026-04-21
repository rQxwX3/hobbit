#include <task_override.hpp>

namespace hbt::mods {
[[nodiscard]] auto TaskOverride::validateUUID(const uuid_t &uuid) -> uuid_t {
    if (uuid.empty()) {
        throw std::invalid_argument(
            "TaskOverride: recurrentTaskUUID can't be empty");
    }

    return uuid;
}

[[nodiscard]] auto
TaskOverride::validateAppliesUntil(appliesUntil_t appliesUntil)
    -> appliesUntil_t {
    if (appliesUntil.has_value() && appliesUntil.value() < appliesOn_) {
        throw std::invalid_argument(
            "TaskOverride: appliesUntil can't be earlier than appliesOn");
    }

    return appliesUntil;
}

TaskOverride::TaskOverride(const uuid_t &recurrentTaskUUID,
                           appliesOn_t appliesOn, appliesUntil_t appliesUntil,
                           OverrideData override)
    : recurrentTaskUUID_{validateUUID(recurrentTaskUUID)},
      appliesOn_{appliesOn}, appliesUntil_{validateAppliesUntil(appliesUntil)},
      override_{std::move(override)} {}

[[nodiscard]] auto TaskOverride::getPriority() const -> Priority {
    if (appliesUntil_.has_value()) {
        return Priority::SliceOverride;
    }

    return Priority::IndividualOverride;
}
} // namespace hbt::mods
