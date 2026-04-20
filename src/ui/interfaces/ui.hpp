#pragma once

#include <datetime.hpp>
#include <duration_units.hpp>
#include <task.hpp>

#include <vector>

namespace hbt::ui {
class UI {
  public:
    using createTaskCallback_t =
        std::function<void(std::string, hbt::mods::Interval)>;

  public:
    using IntervalUnits = hbt::mods::DurationUnits::unit_t;

  public:
    enum class Screen : uint8_t {
        TaskList,
        CreateTask,
        Statistics,
        Settings,
        NONE,
    };

  public:
    virtual ~UI() = default;

  public:
    virtual auto start() -> void = 0;

    virtual auto stop() -> void = 0;

    virtual auto refresh() -> void = 0;

  public:
    virtual auto
    setCreateTaskCallback(const createTaskCallback_t &createTaskCallback)
        -> void = 0;

    virtual auto setTaskList(std::vector<hbt::mods::TaskData> tasks)
        -> void = 0;

    virtual auto populateTaskList(hbt::mods::TaskData task) -> void = 0;
};
} // namespace hbt::ui
