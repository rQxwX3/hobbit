#pragma once

#include <datetime/datetime.hpp>
#include <uuid.hpp>

namespace clndr::ev {
class Instance {
  private:
    core::uuid_t uuid_;
    core::uuid_t templateUUID_;

    dt::DateTime datetime_;
    bool completed_;

  public:
    Instance(core::uuid_t templateUUID, dt::DateTime datetime, bool completed);

    Instance(core::uuid_t uuid, core::uuid_t templateUUID,
             dt::DateTime datetime, bool completed);

  public:
    [[nodiscard]] auto ok() const -> bool;

  public:
    [[nodiscard]] auto getUUID() const -> core::uuid_t;

    [[nodiscard]] auto getTemplateUUID() const -> core::uuid_t;

    [[nodiscard]] auto getDateTime() const -> dt::DateTime;

    [[nodiscard]] auto isCompleted() const -> bool;
};
}; // namespace clndr::ev
