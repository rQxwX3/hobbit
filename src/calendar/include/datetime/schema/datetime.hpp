#pragma once

#include <datetime/constants.hpp>
#include <datetime/datetime.hpp>
#include <datetime/schema/date.hpp>
#include <datetime/schema/time.hpp>
#include <fields.hpp>
#include <schema.hpp>
#include <validation.hpp>

namespace clndr::dt::schema {
struct DateTime : core::schema::Base<DateTime, dt::DateTime> {
    struct RuleSet {};

    enum class FieldID : size_t { value };

    using Model = dt::DateTime;

    using Fields = core::schema::Fields<
        FieldID, core::schema::Field<FieldID::value, "value", &Model::getValue,
                                     Model::value_t>>;

    using Rules = core::validation::ModelRules<Model>;
};

static_assert(core::schema::Concept<DateTime, dt::DateTime>);
}; // namespace clndr::dt::schema
