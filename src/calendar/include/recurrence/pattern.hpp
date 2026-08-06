#pragma once

#include <datetime/datetime.hpp>
#include <datetime/interval.hpp>

#include <nlohmann/json.hpp>

namespace clndr::rec {
class Pattern {
  public:
    using occurrence_t = dt::DateTime;
    using occurrences_t = std::vector<occurrence_t>;

  public:
    Pattern() = default;

  public:
    Pattern(const Pattern &) = default;
    auto operator=(const Pattern &) -> Pattern & = default;

    Pattern(Pattern &&) = delete;
    auto operator=(Pattern &&) -> Pattern & = delete;

    [[nodiscard]] auto operator==(const Pattern &other) const -> bool = default;

  public:
    virtual ~Pattern() = default;

  public:
    [[nodiscard]] virtual auto getOccurrencesOfDate(dt::Date date,
                                                    dt::DateTime start) const
        -> occurrences_t = 0;

    [[nodiscard]] virtual auto happensOnDate(dt::Date date,
                                             dt::DateTime start) const
        -> bool = 0;
};
} // namespace clndr::rec
