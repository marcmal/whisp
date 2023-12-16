#pragma once

#include "util/types.hpp"
#include <span>
#include <string>
#include <utility>
#include <vector>

namespace fict_tele
{

class Decoder
{
  public:
    Decoder(const std::span<Byte>& data, const int bitsPerChannel);
    std::pair<std::string, std::vector<Byte>> decode();

  private:
    std::size_t decodeLength();
    std::vector<Byte> decodeData(std::size_t length);
    Byte decodeByte();

    const std::span<Byte> data;
    const int bitsPerChannel;
    std::span<Byte>::iterator iterator;
};

}
