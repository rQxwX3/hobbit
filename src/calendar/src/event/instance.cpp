#include <event/instance.hpp>

namespace clndr::ev {
Instance::Instance(core::uuid_t templateUUID, dt::DateTime datetime,
                   bool completed)
    : uuid_{core::generateUUID()}, templateUUID_{std::move(templateUUID)},
      datetime_{datetime}, completed_{completed} {}

Instance::Instance(core::uuid_t uuid, core::uuid_t templateUUID,
                   dt::DateTime datetime, bool completed)
    : uuid_{std::move(uuid)}, templateUUID_{std::move(templateUUID)},
      datetime_{datetime}, completed_{completed} {};

[[nodiscard]] auto Instance::getUUID() const -> core::uuid_t { return uuid_; }

[[nodiscard]] auto Instance::getTemplateUUID() const -> core::uuid_t {
    return templateUUID_;
}

[[nodiscard]] auto Instance::getDateTime() const -> dt::DateTime {
    return datetime_;
}

[[nodiscard]] auto Instance::isCompleted() const -> bool { return completed_; }
} // namespace clndr::ev
