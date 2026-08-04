#pragma once

#include <optional>

#include <datetime/datetime.hpp>

namespace clndr::dt {
class OptDateTime {
  public:
    using value_t = std::optional<DateTime>;

  public:
    struct Error : core::err::Base<Error> {
        static constexpr auto className{std::string_view{"dt::OptDateTime"}};

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
    value_t value_;

  public:
    OptDateTime(value_t optDateTime);

  public:
    [[nodiscard]] auto ok() const -> bool;

  public:
    [[nodiscard]] auto hasValue() const -> bool;

    [[nodiscard]] auto getValue() const -> DateTime;

    [[nodiscard]] auto getOptional() const -> value_t;

  public:
    [[nodiscard]] auto operator==(const OptDateTime &other) const
        -> bool = default;
};
} // namespace clndr::dt
