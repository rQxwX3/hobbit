#include <codec/json/recurrence/null_pattern.hpp>
#include <recurrence/null_pattern.hpp>

namespace clndr::codec::json::rec {
[[nodiscard]] auto NullPattern::encode(const clndr::rec::NullPattern &pattern)
    -> nlohmann::json {
    return value;
}

[[nodiscard]] auto NullPattern::decode(const nlohmann::json &json)
    -> std::expected<clndr::rec::NullPattern, Error::Code> {
    if (json.get<std::string>() != value) {
        return std::unexpected(Error::Code::FailedToParse);
    }

    return clndr::rec::NullPattern();
}
} // namespace clndr::codec::json::rec
