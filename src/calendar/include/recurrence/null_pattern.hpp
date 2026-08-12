#pragma once

#include <recurrence/pattern.hpp>

#include <nlohmann/json.hpp>

namespace clndr::rec {
class NullPattern : public pattern::Base {
  public:
    static constexpr auto type{pattern::Type::Null};

  public:
    NullPattern() = default;

  public:
    [[nodiscard]] auto getType() const -> pattern::Type override;

    [[nodiscard]] auto getOccurrencesOfDate(dt::Date date,
                                            dt::DateTime start) const
        -> occurrences_t override;

    [[nodiscard]] auto happensOnDate(dt::Date date, dt::DateTime start) const
        -> bool override;
};

static_assert(pattern::Concept<NullPattern>);
} // namespace clndr::rec
