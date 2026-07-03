#pragma once

#include <regex>

#include <datetime/date.hpp>
#include <datetime/datetime.hpp>
#include <datetime/time.hpp>
#include <error.hpp>
#include <iso8601.hpp>

namespace clndr::codec::iso8601 {
struct DateTime {
    struct Error : core::err::Base<Error> {
        static constexpr auto className{std::string_view{"ISO8601::DateTime"}};

        enum class Code : uint8_t {
            RegexMismatch,
            UnmatchedUnit,
            FailedToValidateDate,
            FailedToValidateTime,
        };

        [[nodiscard]] static constexpr auto getMessage(Code code)
            -> std::string;
    };

    static_assert(core::err::Concept<Error>);

    static inline const auto regex{std::regex{
        R"(^(\d{4})[-./](\d{2})[-./](\d{2})[ T](\d{2}):(\d{2})(?::(\d{2}))?$)"}};

    [[nodiscard]] static auto encode(const dt::DateTime &datetime)
        -> std::string;

    [[nodiscard]] static auto decode(const std::string &string)
        -> std::expected<dt::DateTime, Error::Code>;
};

static_assert(core::codec::iso8601::Concept<DateTime, dt::DateTime>);
}; // namespace clndr::codec::iso8601
