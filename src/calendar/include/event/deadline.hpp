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

  public:
    enum class Error : uint8_t {
        InvalidUnderlyingType,
        RTInvalidUnderlyingType,

        IntervalBadAccess,
        DateTimeBadAccess,
    };

  public:
    [[nodiscard]] static constexpr auto errorMessage(Error error)
        -> std::string {
        switch (error) {

        case Error::InvalidUnderlyingType:
            return "Deadline: provided value is not of a valid deadline type";

        case Error::RTInvalidUnderlyingType:
            return "Deadline: invalid object state (invalid underlying type)";

        case Error::IntervalBadAccess:
            return "Deadline: attempted to access interval in a non-interval "
                   "instance";

        case Error::DateTimeBadAccess:
            return "Deadline: attempted to access datetime in a non-datetime "
                   "instance";

        default:
            std::unreachable();
        }
    }

  private:
    underlying_type_t type_;

  public:
    Deadline(underlying_type_t underlyingType);

  private:
    [[nodiscard]] static auto validateUnderlyingType(underlying_type_t type)
        -> underlying_type_t;

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
