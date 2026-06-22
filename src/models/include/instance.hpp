#pragma once

#include <datetime.hpp>
#include <uuid.hpp>

namespace hbt::mods::task {
class Instance {
  private:
    using uuid_t = core::uuid::uuid_t;

  public:
    uuid_t uuid_;
    uuid_t templateUUID_;

    DateTime datetime_;
    bool completed_;

  public:
    Instance(uuid_t templateUUID, DateTime datetime, bool completed);
};
}; // namespace hbt::mods::task
