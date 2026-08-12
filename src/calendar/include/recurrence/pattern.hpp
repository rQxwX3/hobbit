#pragma once

#include <datetime/datetime.hpp>
#include <datetime/interval.hpp>

#include <nlohmann/json.hpp>

namespace clndr::rec::pattern {
enum class Type : uint8_t {
    Null,
    Interval,
    Weekdays,
};

class Base {
  public:
    using occurrence_t = dt::DateTime;
    using occurrences_t = std::vector<occurrence_t>;

  public:
    Base() = default;

  public:
    Base(const Base &) = default;
    auto operator=(const Base &) -> Base & = default;

    Base(Base &&) = delete;
    auto operator=(Base &&) -> Base & = delete;

    [[nodiscard]] auto operator==(const Base &other) const -> bool = default;

  public:
    virtual ~Base() = default;

  public:
    [[nodiscard]] virtual auto getType() const -> Type = 0;

    [[nodiscard]] virtual auto getOccurrencesOfDate(dt::Date date,
                                                    dt::DateTime start) const
        -> occurrences_t = 0;

    [[nodiscard]] virtual auto happensOnDate(dt::Date date,
                                             dt::DateTime start) const
        -> bool = 0;
};

template <typename P>
concept Concept = requires {
    std::same_as<decltype(P::type), Type>;
    std::derived_from<P, Base>;
};
} // namespace clndr::rec::pattern
