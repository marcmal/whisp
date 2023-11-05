#pragma once

#include "util/types.hpp"
#include <span>
#include <vector>

namespace fict_tele
{

class Encoder
{
  public:
    Encoder(const std::span<Byte>& data, const int bitsPerChannel);
    void encode(const std::vector<Byte>& buffer);

  private:
    void encodeSize(const std::size_t size);
    void encodeByte(const Byte byte);

    const std::span<Byte> data;
    const int bitsPerChannel;
    std::span<Byte>::iterator iterator;
};

}
