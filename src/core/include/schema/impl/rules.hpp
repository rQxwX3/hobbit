#include <algorithm>
#include <ranges>

namespace core::schema::impl::rules {
template <typename Rule> struct Each {
    template <typename Container>
    static auto check(const Container &c) -> bool {
        return std::ranges::all_of(
            c, [](const auto &value) -> auto { return Rule::check(value); });
    }
};

template <auto Max> struct Less {
    template <typename T>
    [[nodiscard]] static auto check(const T &value) -> bool {
        return value < Max;
    }
};

template <auto Min, auto Max> struct Range {
    template <typename T>
    [[nodiscard]] static auto check(const T &value) -> bool {
        return Min <= value && value <= Max;
    }
};

template <auto Min, auto Max> struct EnumRange {
    template <typename E>
        requires std::is_enum_v<E>
    static constexpr auto check(E value) -> bool {
        using U = std::underlying_type_t<E>;
        auto v = static_cast<U>(value);
        return v >= static_cast<U>(Min) && v <= static_cast<U>(Max);
    }
};

struct Ascending {
    template <typename Container>
    static constexpr auto check(const Container &c) -> bool {
        return std::ranges::is_sorted(c, std::ranges::less{});
    }
};

template <typename T>
concept HasNext = requires(const T &t) {
    { t.next() } -> std::same_as<T>;
};

struct ConsecutivelyAscending {
    template <std::ranges::forward_range R>
        requires HasNext<std::ranges::range_value_t<R>>
    static constexpr auto check(const R &r) -> bool {
        auto it{std::ranges::begin(r)};
        auto end{std::ranges::end(r)};

        if (it == end) {
            return true;
        }

        auto prev{it++};
        for (; it != end; ++it, ++prev) {
            if (prev->next() != *it) {
                return false;
            }
        }

        return true;
    }
};

template <typename Model> struct ValidOptional {
    template <typename Optional>
        requires std::same_as<Optional, std::optional<Model>>
    static constexpr auto check(const Optional &opt) -> bool {
        if (opt.has_value()) {
            return opt->ok();
        }

        return true;
    }
};
}; // namespace core::schema::impl::rules
