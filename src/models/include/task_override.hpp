#pragma once

#include <datetime.hpp>
#include <task_data.hpp>
#include <uuid.hpp>

#include <optional>

namespace hbt::mods {
class TaskOverride {
  public:
    using uuid_t = core::uuid::uuid_t;

    using appliesOn_t = hbt::mods::DateTime;
    using appliesUntil_t = std::optional<hbt::mods::DateTime>;

    using title_t = std::optional<std::string>;
    using deadline_t = std::optional<TaskData::deadline_t>;
    using cancelled_t = std::optional<bool>;
    using startFrom_t = std::optional<TaskData::start_t>;

    struct OverrideData {
        title_t title_;
        deadline_t deadline_;
        cancelled_t cancelled_;
        startFrom_t startFrom_;
    };

    enum class Priority : uint8_t {
        SliceOverride,
        IndividualOverride,
    };

  private:
    uuid_t recurrentTaskUUID_;

    hbt::mods::DateTime appliesOn_;
    std::optional<hbt::mods::DateTime> appliesUntil_;

    OverrideData override_;

  private:
    [[nodiscard]] auto validateUUID(const uuid_t &uuid) -> uuid_t;
    [[nodiscard]] auto validateAppliesUntil(appliesUntil_t appliesUntil)
        -> appliesUntil_t;

  public:
    TaskOverride(const uuid_t &recurrentTaskUUID, appliesOn_t appliesOn,
                 appliesUntil_t appliesUntil, OverrideData override);

  public:
    [[nodiscard]] auto getPriority() const -> Priority;
};
} // namespace hbt::mods
