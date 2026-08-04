#pragma once

#include <chrono>
#include <cstdint>

#include <datetime/interval.hpp>
#include <error.hpp>

namespace clndr::dt {
class Time {
  public:
    using hour_t = uint8_t;
    using minute_t = uint8_t;

    using duration_t = std::chrono::minutes;

  public:
    struct Error : core::err::Base<Error> {
        static constexpr auto className{std::string_view{"dt::Time"}};

        enum class Code : uint8_t {
            InvalidCtorArgs,
        };

        [[nodiscard]] static constexpr auto getMessage(Code code)
            -> std::string {
            switch (code) {

            case Code::InvalidCtorArgs:
                return generateMessage(
                    "cannot instantiate valid object from provided arguments");

            default:
                std::unreachable();
            }
        }

        static_assert(core::err::Concept<Error>);
    };

  private:
    /* order must not be changed */
    hour_t hour_;
    minute_t minute_;

  public:
    Time(hour_t hour, minute_t minute);

  public:
    [[nodiscard]] auto ok() const -> bool;

  public:
    [[nodiscard]] static auto midnight() -> Time;

  public:
    [[nodiscard]] auto getHour() const -> hour_t;

    [[nodiscard]] auto getMinute() const -> minute_t;

  public:
    [[nodiscard]] auto toDuration() const -> duration_t;

    [[nodiscard]] auto operator<=>(const Time &other) const
        -> std::strong_ordering = default;
};
} // namespace clndr::dt
