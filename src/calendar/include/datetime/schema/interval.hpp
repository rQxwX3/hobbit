#pragma once

#include <datetime/interval.hpp>
#include <fields.hpp>
#include <schema.hpp>
#include <validation.hpp>

namespace clndr::dt::schema {
struct Interval : core::schema::Base<Interval, dt::Interval> {
    struct RuleSet {};

    enum class FieldID : size_t { array, monthHandling };

    using Model = dt::Interval;

    using Fields = core::schema::Fields<
        FieldID,
        core::schema::Field<FieldID::array, "array", &Model::getArray,
                            Model::array_t,
                            core::validation::Less<dt::Interval::maxValue>>,
        core::schema::Field<FieldID::monthHandling, "month_handling",
                            &Model::getMonthHandling, Model::MonthHandling>>;

    using Rules = core::validation::ModelRules<Model>;
};

static_assert(core::schema::Concept<Interval, dt::Interval>);
}; // namespace clndr::dt::schema
