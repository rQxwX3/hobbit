#pragma once

#include <datetime/date.hpp>
#include <fields.hpp>
#include <schema.hpp>
#include <validation.hpp>

namespace clndr::schema {
struct Date {
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
        core::schema::Field<FieldID::year, "year", Model::year_t,
                            core::validation::FieldRules<>>,
        core::schema::Field<FieldID::month, "month", Model::month_t,
                            core::validation::FieldRules<>>,
        core::schema::Field<FieldID::day, "day", Model::day_t,
                            core::validation::FieldRules<>>>;

    using Rules =
        core::validation::ModelRules<Model, RuleSet::ValidYearMonthDate>;

    [[nodiscard]] static auto validate(const Model &obj) -> bool {
        return Rules::validate(obj);
    }
};

static_assert(core::schema::Concept<Date, dt::Date>);
}; // namespace clndr::schema
