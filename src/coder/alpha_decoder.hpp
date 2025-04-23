#pragma once

#include "decoder.hpp"
#include "util/types.hpp"
#include <expected>
#include <span>
#include <vector>

namespace whisp
{
class AlphaDecoder : public Decoder
{
  public:
    AlphaDecoder(const std::span<Byte>& data);
    DecodeResult decode() override;

  private:
    std::size_t decodeLength();
    std::vector<Byte> decodeData(std::size_t length);
    Byte decodeByte();

    std::span<Byte> buffer;
    std::span<Byte>::iterator iterator;
};

}
