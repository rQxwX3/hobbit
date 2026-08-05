#include <event/error/instance.hpp>
#include <event/instance.hpp>
#include <event/schema/instance.hpp>

namespace clndr::ev {
Instance::Instance(core::uuid_t templateUUID, dt::DateTime datetime,
                   bool completed)
    : uuid_{core::generateUUID()}, templateUUID_{std::move(templateUUID)},
      datetime_{datetime}, completed_{completed} {
    if (!ok()) {
        throw std::invalid_argument(
            std::string(error::instance::InvalidCtorArgs::msg));
    }
}

Instance::Instance(core::uuid_t uuid, core::uuid_t templateUUID,
                   dt::DateTime datetime, bool completed)
    : uuid_{std::move(uuid)}, templateUUID_{std::move(templateUUID)},
      datetime_{datetime}, completed_{completed} {
    if (!ok()) {
        throw std::invalid_argument(
            std::string(error::instance::InvalidCtorArgs::msg));
    }
};

[[nodiscard]] auto Instance::ok() const -> bool {
    return schema::instance::Schema::validate(*this);
}

[[nodiscard]] auto Instance::getUUID() const -> core::uuid_t { return uuid_; }

[[nodiscard]] auto Instance::getTemplateUUID() const -> core::uuid_t {
    return templateUUID_;
}

[[nodiscard]] auto Instance::getDateTime() const -> dt::DateTime {
    return datetime_;
}

[[nodiscard]] auto Instance::isCompleted() const -> bool { return completed_; }
} // namespace clndr::ev
