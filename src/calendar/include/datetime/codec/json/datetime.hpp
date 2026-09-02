#include <regex>

#include <codec/codec.hpp>
#include <datetime/datetime.hpp>

namespace clndr::dt::codec::json::datetime {
namespace processors {
namespace iso8601 {
struct Encoder {
    static constexpr std::string format{"{:%Y-%m-%dT%H:%M}"};

    [[nodiscard]] static auto operator()(const dt::DateTime &datetime)
        -> std::string;
};

struct Decoder {
    struct Error {
        static constexpr auto className{core::FixedString{
            "clndr::dt::codec::json::datetime::processors::iso8601::Decoder"}};

        enum class Code : uint8_t {
            RegexMismatch,
            UnmatchedDateOrTime,
            ConstructionFailed,
        };

        using RegexMismatch =
            core::error::Error<Code::RegexMismatch, std::runtime_error,
                               className, "provided input did not match regex">;

        using UnmatchedDateOrTime =
            core::error::Error<Code::UnmatchedDateOrTime, std::runtime_error,
                               className,
                               "provided input doesn't contain one of the "
                               "following: dt::Date, dt::Time">;

        using ConstructionFailed = core::error::Error<
            Code::ConstructionFailed, std::runtime_error, className,
            "failed to construct a valid object from decoded data">;
    };

    static inline const std::regex regex{
        R"(^(\d{4}-\d{2}-\d{2})[ T](\d{2}:\d{2})$)"};

    [[nodiscard]] static auto operator()(const std::string &string)
        -> dt::DateTime;
}; // namespace error
} // namespace iso8601
} // namespace processors

namespace fields {
using iso8601 = core::codec::fields::Field<
    dt::DateTime, std::string, processors::iso8601::Encoder,
    processors::iso8601::Decoder,
    [](dt::DateTime &datetime) -> dt::DateTime & { return datetime; },
    [](nlohmann::json &json) -> std::string & {
        return json.get_ref<std::string &>();
    }>;

using all = core::codec::fields::Fields<iso8601>;
}; // namespace fields

using Codec = core::codec::Codec<dt::DateTime, nlohmann::json, fields::all>;
static_assert(
    core::codec::concepts::Codec<Codec, dt::DateTime, nlohmann::json>);
} // namespace clndr::dt::codec::json::datetime
