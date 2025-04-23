#pragma once

#include "decoder.hpp"
#include "util/types.hpp"
#include <expected>
#include <span>
#include <vector>

namespace whisp
{
class RgbDecoder : public Decoder
{
  public:
    RgbDecoder(const std::span<Byte>& data, const int bitsPerChannel);
    DecodeResult decode() override;

  private:
    std::size_t decodeLength();
    std::vector<Byte> decodeData(std::size_t length);
    Byte decodeByte();

    std::span<Byte> buffer;
    std::span<Byte>::iterator iterator;
    int bitsPerChannel;
};

}
