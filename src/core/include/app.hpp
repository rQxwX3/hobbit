#pragma once

#include <date.hpp>
#include <entry_service.hpp>
#include <ui.hpp>

#include <memory>

namespace hbt::core {
class App {
  private:
    std::unique_ptr<hbt::core::EntryService> entries_;
    std::unique_ptr<hbt::ui::UI> ui_;
    hbt::mods::Date date_;

  public:
    App(std::unique_ptr<hbt::core::EntryService> entries,
        std::unique_ptr<hbt::ui::UI> ui);

  public:
    auto run() -> void;

    auto stop() -> void;

  public:
    auto refreshUI() -> void;
};
} // namespace hbt::core
