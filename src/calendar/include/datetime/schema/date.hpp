#pragma once

#include <datetime/date.hpp>
#include <schema/fields.hpp>
#include <schema/rules.hpp>
#include <schema/schema.hpp>

namespace clndr::dt::schema {
namespace fields {
using namespace core::schema::fields;
using YearField = Field<dt::Date::year_t, dt::Date,
                        [](const dt::Date &date) -> dt::Date::year_t {
                            return date.getYear();
                        },
                        "year">;

using MonthField = Field<dt::Date::month_t, dt::Date,
                         [](const dt::Date &date) -> dt::Date::month_t {
                             return date.getMonth();
                         },
                         "month">;

using DayField =
    Field<dt::Date::day_t, dt::Date,
          [](const dt::Date &date) -> dt::Date::day_t { return date.getDay(); },
          "day">;

using Fields = Fields<YearField, MonthField, DayField>;
}; // namespace fields

namespace rules {
using namespace core::schema::rules;

using ValidYearMonth = Rule<[](const dt::Date &date) -> bool {
    auto ymd{std::chrono::year_month_day(
        std::chrono::year(fields::YearField::accessor(date)),
        std::chrono::month(fields::MonthField::accessor(date)),
        std::chrono::day(fields::DayField::accessor(date)))};

    return ymd.ok();
},
                            fields::Fields>;

using Rules = Rules<ValidYearMonth>;
}; // namespace rules

using DateSchema = core::schema::Schema<dt::Date, fields::Fields, rules::Rules>;
}; // namespace clndr::dt::schema
