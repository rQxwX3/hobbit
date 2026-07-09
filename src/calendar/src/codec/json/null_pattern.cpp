#include <codec/json/null_pattern.hpp>
#include <recurrence/null_pattern.hpp>

namespace clndr::codec::json {
[[nodiscard]] auto NullPattern::encode(const rec::NullPattern &pattern)
    -> nlohmann::json {
    return value;
}

[[nodiscard]] auto NullPattern::decode(const nlohmann::json &json)
    -> std::expected<rec::NullPattern, Error::Code> {
    if (json.get<std::string>() != value) {
        return std::unexpected(Error::Code::FailedToParse);
    }

    return rec::NullPattern();
}
} // namespace clndr::codec::json
