#pragma once

#include "util/types.hpp"
#include <span>
#include <vector>

namespace fict_tele
{

class Decoder
{
  public:
    Decoder(const std::span<Byte>& data, const int bitsPerChannel);
    std::vector<Byte> decode();

  private:
    std::size_t decodeSize();
    Byte decodeByte();

    const std::span<Byte> data;
    const int bitsPerChannel;
    std::span<Byte>::iterator iterator;
};

}
