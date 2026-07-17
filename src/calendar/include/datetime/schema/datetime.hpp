#pragma once

#include <datetime/constants.hpp>
#include <datetime/datetime.hpp>
#include <datetime/schema/date.hpp>
#include <datetime/schema/time.hpp>
#include <fields.hpp>
#include <schema.hpp>
#include <validation.hpp>

namespace clndr::dt::schema {
struct DateTime {
    enum class FieldID : size_t { value };

    using Model = dt::DateTime;

    using Fields = core::schema::Fields<
        FieldID, core::schema::Field<FieldID::value, "value", Model::value_t>>;

    struct RuleSet {};

    using Rules = core::validation::ModelRules<Model>;

    [[nodiscard]] static auto validate(const Model &obj) -> bool {
        return Date::validate(obj.getDate()) && Time::validate(obj.getTime());
    }
};

static_assert(core::schema::Concept<DateTime, dt::DateTime>);
}; // namespace clndr::dt::schema
