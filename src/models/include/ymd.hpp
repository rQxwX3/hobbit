#pragma once

#include <cstdint>
#include <string>

namespace hbt::mods {
class YMD {
  public:
    using year_value_t = int16_t;
    using month_value_t = uint8_t;
    using day_value_t = uint8_t;

  public:
    enum class Error : uint8_t {
        InvalidArguments,
    };

  public:
    [[nodiscard]] static constexpr auto errorMessage(Error error)
        -> std::string {
        switch (error) {
        case Error::InvalidArguments:
            return "YMD: cannot construct valid YMD object from provided "
                   "values";

        default:
            std::unreachable();
        }
    }

  private:
    year_value_t year_;
    month_value_t month_;
    day_value_t day_;

  public:
    YMD(year_value_t year, month_value_t month, day_value_t day);

  private:
    auto validate() const -> void;
};

} // namespace hbt::mods
