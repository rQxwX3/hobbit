#pragma once

#include <nlohmann/json.hpp>

#include <duration.hpp>

#include <array>
#include <expected>

namespace hbt::mods {
class Interval {
  private:
    enum class Error : uint8_t {
        JSONMissingRequiredField,
        JSONFailedToParseDuration,

        NaturalLanguageFailedToParseDuration,
    };

  public:
    [[nodiscard]] static constexpr auto errorMessage(Error error)
        -> std::string {
        switch (error) {
        case Error::JSONMissingRequiredField:
            return "Interval: missing required field(s) in JSON";

        case Error::JSONFailedToParseDuration:
            return "Interval: failed to parse Duration from JSON";

        case Error::NaturalLanguageFailedToParseDuration:
            return "Interval: failed to parse Duration from natural language";
        }
    }

  private:
    static constexpr auto jsonDurationField{std::string_view{"duration"}};
    static constexpr auto jsonMonthHandlingField{
        std::string_view{"month_handling"}};

    static constexpr auto jsonFields{std::array<std::string_view, 2>{
        jsonDurationField, jsonMonthHandlingField}};

  public:
    enum class MonthHandling : char { CUT_OFF, WRAP_AROUND };

  public:
    constexpr static auto defaultMonthHandling{MonthHandling::WRAP_AROUND};

  public:
    using value_t = hbt::mods::Duration::value_t;
    using unit_t = hbt::mods::Duration::unit_t;

  private:
  private:
    hbt::mods::Duration duration_;
    MonthHandling monthHandling_;

  public:
    Interval(hbt::mods::Duration duration,
             MonthHandling monthHandling = defaultMonthHandling);

    Interval(const Interval &other);

  public:
    explicit Interval(
        hbt::mods::Duration::Units units = hbt::mods::Duration::Units{},
        MonthHandling monthHandling = defaultMonthHandling);

  public:
    [[nodiscard]] auto getUnitValue(unit_t unit) const -> value_t;

  public:
    [[nodiscard]] auto getMonthHandling() const -> MonthHandling;

    [[nodiscard]] auto getDuration() const -> Duration;

  public:
    auto setMonthHandling(MonthHandling monthHandling) -> void;

  public:
    [[nodiscard]] auto operator+(const Interval &other) const -> Interval;

    [[nodiscard]] auto operator<=>(const Interval &other) const
        -> std::strong_ordering = default;

    [[nodiscard]] auto operator==(const Interval &other) const
        -> bool = default;

  public:
    [[nodiscard]] auto isZero() const -> bool;

    [[nodiscard]] auto onlyContainsUnit(unit_t unit) const -> bool;

  private:
    [[nodiscard]] auto static containsAllJSONFields(const nlohmann::json &json)
        -> bool;

  public:
    [[nodiscard]] auto toJSON() const -> nlohmann::json;

    [[nodiscard]] static auto fromJSON(const nlohmann::json &json)
        -> std::expected<Interval, Error>;

  public:
    [[nodiscard]] static auto fromNaturalLanguage(const std::string &input)
        -> std::expected<Interval, Error>;

    [[nodiscard]] auto toNaturalLanguage() const -> std::string;
};
} // namespace hbt::mods
