#include <ymd.hpp>

#include <chrono>

namespace hbt::mods {
auto YMD::validate() const -> void {
    using namespace std::chrono;

    auto chrono{year_month_day(year{year_}, month{month_}, day{day_})};

    if (!chrono.ok()) {
        throw std::invalid_argument(errorMessage(Error::InvalidArguments));
    }
}

YMD::YMD(year_value_t year, month_value_t month, day_value_t day)
    : year_{year}, month_{month}, day_{day} {
    validate();
}
}; // namespace hbt::mods
