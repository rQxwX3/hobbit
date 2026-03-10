#include "models/include/entry.hpp"
#include "models/include/user.hpp"

#include <chrono>
#include <fstream>
#include <iostream>

auto main() -> int {
    std::fstream entriesFile{"entries.txt"};
    entriesFile.open("entries.txt");

    auto user{hbt::User()};

    user.addEntry(
        hbt::Entry{"title", {{std::chrono::Sunday, hbt::DayPart::AFTERNOON}}});

    auto entries{user.getEntries()};

    for (const auto &entry : entries) {
        std::cout << entry.getTitle() << '\n';
    }
}
