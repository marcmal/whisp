#pragma once

#include "decoder.hpp"
#include "util/step_by_iterable.hpp"
#include "util/types.hpp"
#include <expected>
#include <span>
#include <vector>

namespace whisp
{
class AlphaDecoder : public Decoder
{
    using InputBuffer = std::span<Byte>;

  public:
    AlphaDecoder(const std::span<Byte>&);
    DecodeResult decode() override;

  private:
    std::size_t decodeLength();
    std::vector<Byte> decodeData(std::size_t length);
    Byte decodeByte();

    decltype(step_by::makeIterable<4>(std::declval<InputBuffer>())) buffer;
    decltype(buffer.cursor()) cursor;
};

}
