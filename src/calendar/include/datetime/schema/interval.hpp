#include <datetime/error/interval.hpp>
#include <datetime/interval.hpp>
#include <schema/schema.hpp>

namespace clndr::dt::schema::interval {
namespace fields {
using namespace core::schema::fields;
using Array = Field<dt::Interval::array_t,
                    [](const dt::Interval &interval) -> dt::Interval::array_t {
                        return interval.getArray();
                    }>;

using MonthHandling =
    Field<dt::Interval::MonthHandling,
          [](const dt::Interval &interval) -> dt::Interval::MonthHandling {
              return interval.getMonthHandling();
          }>;

using all = Fields<Array, MonthHandling>;
}; // namespace fields

namespace rules {
using namespace core::schema::rules;
using ValidArray = Rule<[](const dt::Interval &interval) -> bool {
    const auto array{fields::Array::accessor(interval)};

    return std::ranges::all_of(array,
                               [](const dt::Interval::value_t &value) -> bool {
                                   return value < dt::Interval::maxValue;
                               });
},
                        error::interval::InvalidArray, fields::Array>;
using ValidMonthHandling =
    Rule<[](const dt::Interval &interval) -> bool {
        const auto value{fields::MonthHandling::accessor(interval)};

        return Interval::isValidMonthHandling(
            fields::MonthHandling::accessor(interval));
    },
         error::interval::InvalidMonthHandling, fields::MonthHandling>;

using all = Rules<ValidArray, ValidMonthHandling>;
}; // namespace rules

using Schema = core::schema::Schema<dt::Interval, fields::all, rules::all>;
}; // namespace clndr::dt::schema::interval
