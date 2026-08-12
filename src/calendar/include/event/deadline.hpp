#pragma once

#include <datetime/datetime.hpp>
#include <datetime/interval.hpp>

#include <nlohmann/json.hpp>

#include <variant>

namespace clndr::ev {
class Deadline {
  public:
    enum class Type : uint8_t {
        Interval,
        DateTime,
        Null,
    };

    using underlying_type_t =
        std::variant<dt::Interval, dt::DateTime, std::monostate>;

  private:
    underlying_type_t type_;

  public:
    Deadline();

    Deadline(underlying_type_t underlyingType);

  public:
    [[nodiscard]] static auto null() -> Deadline;

    [[nodiscard]] auto isNull() const -> bool;

    [[nodiscard]] auto isInterval() const -> bool;

    [[nodiscard]] auto isDateTime() const -> bool;

  public:
    [[nodiscard]] auto getUnderlyingType() const -> underlying_type_t;

    [[nodiscard]] auto getType() const -> Type;

    [[nodiscard]] auto getInterval() const -> dt::Interval;

    [[nodiscard]] auto getDateTime() const -> dt::DateTime;

  public:
    [[nodiscard]] auto operator==(const Deadline &deadline) const -> bool;
};
} // namespace clndr::ev
