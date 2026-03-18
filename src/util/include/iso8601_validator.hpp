#pragma once

#include <string>

namespace hbt::util {
class ISO8601Validator {
  private:
    enum class ErrorCode : uint8_t {
        NONE,

        EMPTY_STRING,
        MISSING_P_PREFIX,

        REGEX_MISMATCH,
    };

    class Result {
      private:
        bool valid_;
        std::string errorMessage_;
        size_t errorPosition_;

      private:
        Result(bool valid, ISO8601Validator::ErrorCode errorCode,
               size_t errorPosition)
            : valid_{valid},

              errorMessage_{
                  ISO8601Validator::errorCodeToDescription(errorCode)},

              errorPosition_{errorPosition} {}

      public:
        static auto success() -> Result {
            return Result{true, ErrorCode::NONE, std::string::npos};
        }

        static auto failure(ErrorCode errorCode, size_t errorPosition)
            -> Result {
            return Result{false, errorCode, errorPosition};
        }

        explicit operator bool() const { return valid_; }
    };

  public:
    ISO8601Validator() = delete;

  public:
    [[nodiscard]] auto static validate(const std::string &string) -> Result;

  private:
    [[nodiscard]] static auto isNotEmpty(const std::string &string) -> Result;

    [[nodiscard]] static auto startsWithP(const std::string &string) -> Result;

    [[nodiscard]] static auto matchesRegex(const std::string &string) -> Result;

  private:
    [[nodiscard]] static auto errorCodeToDescription(ErrorCode errorCode)
        -> std::string;
};
} // namespace hbt::util
