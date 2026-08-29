#pragma once

#include <codec/accessor.hpp>
#include <codec/processor.hpp>
#include <concepts.hpp>
#include <fixed_string.hpp>

namespace core::codec::fields {
template <typename Model, typename Representation, typename ModelType,
          typename RepresentationType, typename ModelAccessor,
          typename RepresentationAccessor, typename Encoder, typename Decoder>
    requires processor::Concept<Encoder, ModelType, RepresentationType> &&
             processor::Concept<Decoder, RepresentationType, ModelType> &&
             accessor::Concept<ModelAccessor, Model, ModelType> &&
             accessor::Concept<RepresentationAccessor, Representation,
                               RepresentationType>
struct Field {
    using modelAccessor = ModelAccessor;
    using representationAccessor = RepresentationAccessor;

    using modelType = ModelType;
    using representationType = RepresentationType;

    using encoder = Encoder;
    using decoder = Decoder;
}; // namespace core::codec::fields

template <typename... Fs> using Fields = std::tuple<Fs...>;
}; // namespace core::codec::fields
