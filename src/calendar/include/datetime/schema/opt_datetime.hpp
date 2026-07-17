#pragma once

#include <datetime/opt_datetime.hpp>
#include <fields.hpp>
#include <schema.hpp>
#include <validation.hpp>

namespace clndr::dt::schema {
struct OptDateTime : core::schema::Base<OptDateTime, dt::OptDateTime> {
    struct RuleSet {};

    enum class FieldID : size_t {
        value,
    };

    using Model = dt::OptDateTime;

    using Fields = core::schema::Fields<
        FieldID,
        core::schema::Field<FieldID::value, "value", &Model::getOptional,
                            Model::value_t,
                            core::validation::ValidOptional<dt::DateTime>>>;

    using Rules = core::validation::ModelRules<Model>;
};

static_assert(core::schema::Concept<OptDateTime, dt::OptDateTime>);
}; // namespace clndr::dt::schema
