#pragma once

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
                        fields::Title>;

using ValidDeadline = Rule<[](const ev::Template &templ) -> bool {
    return ev::schema::deadline::Schema::validate(
        fields::Deadline::accessor(templ));
},
                           fields::Deadline>;

using ValidRecurrence = Rule<[](const ev::Template &templ) -> bool {
    return rec::schema::recurrence::Schema::validate(
        fields::Recurrence::accessor(templ));
},
                             fields::Recurrence>;

using ValidDeadlineRecurrenceRelation =
    Rule<[](const ev::Template &templ) -> bool {
        const auto deadlineValue{fields::Deadline::accessor(templ)};
        const auto recurrenceValue{fields::Recurrence::accessor(templ)};

        if (recurrenceValue.isNullPattern()) {
            if (deadlineValue.isDateTime()) {
                return recurrenceValue.getStartDateTime() <
                       deadlineValue.getDateTime();
            }

            return true;
        }

        return !deadlineValue.isDateTime();
    },
         fields::Deadline, fields::Recurrence>;

using all = Rules<ValidTitle, ValidDeadline, ValidRecurrence,
                  ValidDeadlineRecurrenceRelation>;
}; // namespace rules

using Schema = core::schema::Schema<ev::Template, fields::all, rules::all>;
}; // namespace clndr::ev::schema::templ
