#pragma once

#include <codec/processor.hpp>
#include <concepts.hpp>
#include <fixed_string.hpp>

namespace core::codec::fields {
template <FixedString Name, typename Type, typename Format, typename Accessor,
          typename Encoder, typename Decoder>
    requires processor::Concept<Encoder, Type, Format> &&
             processor::Concept<Decoder, Format, Type>
struct Field {
    static constexpr auto name{Name};

    using accessor = Accessor;
    using encoder = Encoder;
    using decoder = Decoder;
};

template <typename... Fs> using Fields = std::tuple<Fs...>;
}; // namespace core::codec::fields
