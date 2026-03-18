#include <iso8601_validator.hpp>

#include <regex>

namespace hbt::util {
[[nodiscard]] auto ISO8601Validator::validate(const std::string &string)
    -> Result {
    if (auto isNotEmptyRes{isNotEmpty(string)}; !isNotEmptyRes) {
        return isNotEmptyRes;
    }

    if (auto startsWithPRes{startsWithP(string)}; !startsWithPRes) {
        return startsWithPRes;
    }

    if (auto matchesRegexRes{matchesRegex(string)}; !matchesRegexRes) {
        return matchesRegexRes;
    }

    return Result::success();
}

[[nodiscard]] auto ISO8601Validator::isNotEmpty(const std::string &string)
    -> Result {
    if (string.empty()) {
        return Result::failure(ErrorCode::EMPTY_STRING, 0);
    }

    return Result::success();
}

[[nodiscard]] auto ISO8601Validator::startsWithP(const std::string &string)
    -> Result {
    if (string[0] == 'P') {
        return Result::success();
    }

    return Result::failure(ErrorCode::MISSING_P_PREFIX, 0);
}

[[nodiscard]] auto ISO8601Validator::matchesRegex(const std::string &string)
    -> Result {
    if (string == "P0D" || string == "PT0M" || string == "P0Y0M0D") {
        return Result::success();
    }

    static const std::regex pattern(
        R"(^P(?=.)((\d+Y)?(\d+M)?(\d+W)?(\d+D)?)?(T(?=\d)(\d+H)?(\d+M)?(\d+S)?)?$)");

    if (std::regex_match(string, pattern)) {
        return Result::success();
    }

    return Result::failure(ErrorCode::REGEX_MISMATCH, 0);
    // return findDetailedError(str);
}

[[nodiscard]] auto ISO8601Validator::errorCodeToDescription(ErrorCode errorCode)
    -> std::string {
    switch (errorCode) {
    case ErrorCode::NONE:
        return "";

    case ErrorCode::EMPTY_STRING:
        return "ISO8601 string cannot be empty";

    case ErrorCode::MISSING_P_PREFIX:
        return "ISO8601 string must be prefixed with 'P'";

    case ErrorCode::REGEX_MISMATCH:
        return "provided string did not match with ISO8601 regular expression";
    }
}
} // namespace hbt::util
