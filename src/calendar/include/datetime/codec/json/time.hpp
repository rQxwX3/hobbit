#include <regex>

#include <codec/codec.hpp>
#include <datetime/time.hpp>

namespace clndr::dt::codec::json::time {
namespace processors {
namespace iso8601 {
struct Encoder {
    static constexpr std::string format{"{:02}:{:02}"};

    [[nodiscard]] static auto operator()(const dt::Time &time) -> std::string;
};

struct Decoder {
    struct Error {
        static constexpr auto className{core::FixedString{
            "clndr::dt::codec::json::time::processors::iso8601::Decoder"}};

        enum class Code : uint8_t {
            RegexMismatch,
            UnmatchedUnit,
            ConstructionFailed,
        };

        using RegexMismatch =
            core::error::Error<Code::RegexMismatch, std::runtime_error,
                               className, "provided input did not match regex">;

        using UnmatchedUnit =
            core::error::Error<Code::UnmatchedUnit, std::runtime_error,
                               className,
                               "provided input does not contain one of the "
                               "following: hour, minute">;

        using ConstructionFailed = core::error::Error<
            Code::ConstructionFailed, std::runtime_error, className,
            "failed to construct a valid object from decoded data">;
    };

    static inline const std::regex regex{R"(^(\d{2}):(\d{2})$)"};

    [[nodiscard]] static auto operator()(const std::string &string) -> dt::Time;
}; // namespace error
} // namespace iso8601
} // namespace processors

namespace fields {
using iso8601 = core::codec::fields::Field<
    dt::Time, std::string, processors::iso8601::Encoder,
    processors::iso8601::Decoder,
    [](dt::Time &time) -> dt::Time & { return time; },
    [](nlohmann::json &json) -> std::string & {
        return json.get_ref<std::string &>();
    }>;

using all = core::codec::fields::Fields<iso8601>;
}; // namespace fields

using Codec = core::codec::Codec<dt::Time, nlohmann::json, fields::all>;
static_assert(core::codec::concepts::Codec<Codec, dt::Time, nlohmann::json>);
} // namespace clndr::dt::codec::json::time
