#pragma once

#include <datetime/week.hpp>
#include <fields.hpp>
#include <schema.hpp>
#include <validation.hpp>

namespace clndr::dt::schema {
struct Week : core::schema::Base<Week, dt::Week> {
    struct RuleSet {};

    enum class FieldID : size_t {
        array,
    };

    using Model = dt::Week;

    using Fields = core::schema::Fields<
        FieldID,

        core::schema::Field<FieldID::array, "array", &Model::toArray,
                            Model::array_t,
                            core::validation::ConsecutivelyAscending>>;

    using Rules = core::validation::ModelRules<Model>;
};

static_assert(core::schema::Concept<Week, dt::Week>);
}; // namespace clndr::dt::schema
