#pragma once

#include <string>

namespace hbt::util {
class ISO8601Validator {
  public:
    enum class ErrorCode : uint8_t {
        EMPTY_STRING,
        MISSING_P_PREFIX,
        MISSING_T_PREFIX,
        DESIGNATOR_WITHOUT_NUMBER,
        NUMBER_WITHOUT_DESIGNATOR
    };

    struct Result {
        bool valid;
        std::string error;
        size_t errorPosition;

        explicit operator bool() const { return valid; }
    };

  public:
    ISO8601Validator() = delete;

  public:
    [[nodiscard]] auto static validate(const std::string &string) -> Result;

  private:
    [[nodiscard]] static auto startsWithP(const std::string &string) -> Result;

    [[nodiscard]] static auto validTimeSection(const std::string &string)
        -> Result;

    [[nodiscard]] static auto
    designatorsFollowedByNumbers(const std::string &string) -> Result;

  private:
    [[nodiscard]] static auto errorCodeToDescription(ErrorCode errorCode)
        -> std::string;
};
} // namespace hbt::util
