#pragma once

#include "encoder.hpp"
#include "util/step_by_iterable.hpp"
#include "util/types.hpp"
#include <expected>

namespace whisp
{

class AlphaEncoder : public Encoder
{
    using InputBuffer = std::span<Byte>;

  public:
    explicit AlphaEncoder(const InputBuffer& data);

  private:
    std::size_t maxBytesToEncode() const override;
    void encode(const Byte byte) override;

    decltype(step_by::makeIterable<4>(std::declval<InputBuffer>())) buffer;
    decltype(buffer.cursor()) cursor;
};

}
