#pragma once

#include <event/error/template.hpp>
#include <event/schema/deadline.hpp>
#include <event/template.hpp>
#include <recurrence/schema/recurrence.hpp>
#include <schema/schema.hpp>

namespace clndr::ev::schema::templ {
namespace fields {
using namespace core::schema::fields;
using Title = Field<std::string, [](const ev::Template &templ) -> std::string {
    return templ.getTitle();
}>;

using Deadline =
    Field<ev::Deadline, [](const ev::Template &templ) -> ev::Deadline {
        return templ.getDeadline();
    }>;

using Recurrence =
    Field<rec::Recurrence, [](const ev::Template &templ) -> rec::Recurrence {
        return templ.getRecurrence();
    }>;

using all = Fields<Title, Deadline, Recurrence>;
}; // namespace fields

namespace rules {
using namespace core::schema::rules;
using ValidTitle = Rule<[](const ev::Template &templ) -> bool {
    return !fields::Title::accessor(templ).empty();
},
                        error::templ::InvalidTitle, fields::Title>;

using ValidRecurrenceDeadlineRelationship =
    Rule<[](const ev::Template &templ) -> bool {
        const auto deadline{fields::Deadline::accessor(templ)};
        const auto recurrence{fields::Recurrence::accessor(templ)};

        if (recurrence.isNullPattern()) {
            if (deadline.isDateTime()) {
                return recurrence.getStartDateTime() < deadline.getDateTime();
            }

            return true;
        }

        return !deadline.isDateTime();
    },
         error::templ::InvalidRecurrenceDeadlineRelationship, fields::Deadline,
         fields::Recurrence>;

using all = Rules<ValidRecurrenceDeadlineRelationship>;
}; // namespace rules

using Schema = core::schema::Schema<ev::Template, fields::all, rules::all>;
}; // namespace clndr::ev::schema::templ
