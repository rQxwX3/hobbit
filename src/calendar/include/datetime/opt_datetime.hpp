#pragma once

#include <optional>

#include <datetime/datetime.hpp>

namespace clndr::dt {
class OptDateTime {
  public:
    using value_t = std::optional<DateTime>;

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
