#pragma once

#include <datetime/date.hpp>
#include <fields.hpp>
#include <schema.hpp>
#include <validation.hpp>

namespace clndr::dt::schema {
struct Date : core::schema::Base<Date, dt::Date> {
    struct RuleSet {
        struct ValidYearMonthDate {
            [[nodiscard]] static auto check(const dt::Date &date) -> bool {
                auto ymd{std::chrono::year_month_day(
                    std::chrono::year(date.getYear()),
                    std::chrono::month(date.getMonth()),
                    std::chrono::day(date.getDay()))};

                return ymd.ok();
            }
        };

        static_assert(
            core::validation::ModelRule<ValidYearMonthDate, dt::Date>);
    };

    enum class FieldID : size_t {
        year,
        month,
        day,
    };

    using Model = dt::Date;

    using Fields = core::schema::Fields<
        FieldID,
        core::schema::Field<FieldID::year, "year", &Model::getYear,
                            Model::year_t>,
        core::schema::Field<FieldID::month, "month", &Model::getMonth,
                            Model::month_t>,
        core::schema::Field<FieldID::day, "day", &Model::getDay, Model::day_t>>;

    using Rules =
        core::validation::ModelRules<Model, RuleSet::ValidYearMonthDate>;
};

static_assert(core::schema::Concept<Date, dt::Date>);
}; // namespace clndr::dt::schema
