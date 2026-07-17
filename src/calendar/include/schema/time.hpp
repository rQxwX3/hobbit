#pragma once

#include <datetime/constants.hpp>
#include <datetime/time.hpp>
#include <fields.hpp>
#include <schema.hpp>
#include <validation.hpp>

namespace clndr::schema {
struct Time {
    enum class FieldID : size_t {
        hour,
        minute,
    };

    using Model = dt::Time;

    using Fields = core::schema::Fields<
        FieldID,

        core::schema::Field<
            FieldID::hour, "hour", Model::hour_t,
            core::validation::Range<dt::constants::minHourValue,
                                    dt::constants::maxHourValue>>,

        core::schema::Field<
            FieldID::minute, "minute", Model::minute_t,
            core::validation::Range<dt::constants::minMinuteValue,
                                    dt::constants::maxMinuteValue>>

        >;

    struct RuleSet {
        struct ValidHourMinute {
            [[nodiscard]] static auto check(const dt::Time &time) -> bool {
                return Fields::template rules<FieldID::hour>::validate(
                           time.getHour()) &&
                       Fields::template rules<FieldID::minute>::validate(
                           time.getMinute());
            }
        };
    };

    using Rules = core::validation::ModelRules<Model, RuleSet::ValidHourMinute>;

    [[nodiscard]] static auto validate(const Model &obj) -> bool {
        return Rules::validate(obj);
    }
};

static_assert(core::schema::Concept<Time, dt::Time>);
}; // namespace clndr::schema
