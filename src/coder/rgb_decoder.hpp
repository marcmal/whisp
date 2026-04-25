#pragma once

#include "decoder.hpp"
#include "util/types.hpp"
#include <span>
#include <vector>

namespace whisp
{
class RgbDecoder : public Decoder
{
  public:
    RgbDecoder(const std::span<Byte>& data, const int bitsPerChannel);

  private:
    std::size_t decodeLength() override;
    std::vector<Byte> decodeData(std::size_t length) override;
    Byte decodeByte() override;

    std::span<Byte> buffer;
    std::span<Byte>::iterator iterator;
    int bitsPerChannel;
};

}
