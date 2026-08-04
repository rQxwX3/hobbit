#include <datetime/interval.hpp>
#include <schema/schema.hpp>

namespace clndr::dt::schema::interval {
namespace fields {
using namespace core::schema::fields;
using MonthHandling =
    Field<dt::Interval::MonthHandling,
          [](const dt::Interval &interval) -> dt::Interval::MonthHandling {
              return interval.getMonthHandling();
          }>;

using Array = Field<dt::Interval::array_t,
                    [](const dt::Interval &interval) -> dt::Interval::array_t {
                        return interval.getArray();
                    }>;

using all = Fields<MonthHandling, Array>;
}; // namespace fields

namespace rules {
using namespace core::schema::rules;
using ValidMonthHandling = Rule<[](const dt::Interval &interval) -> bool {
    const auto value{fields::MonthHandling::accessor(interval)};

    return Interval::isValidMonthHandling(
        fields::MonthHandling::accessor(interval));
},
                                fields::MonthHandling>;

using all = Rules<ValidMonthHandling>;
}; // namespace rules

using Schema = core::schema::Schema<dt::Interval, fields::all, rules::all>;
}; // namespace clndr::dt::schema::interval
