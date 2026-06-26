#include <task/instance.hpp>

namespace hbt::mods::task {
Instance::Instance(uuid_t templateUUID, DateTime datetime, bool completed)
    : uuid_{core::uuid::generateUUID()}, templateUUID_{std::move(templateUUID)},
      datetime_{datetime}, completed_{completed} {}
} // namespace hbt::mods::task
