#pragma once

#include "encoder.hpp"
#include "util/types.hpp"
#include <expected>

namespace whisp
{

class RgbEncoder : public Encoder
{
  public:
    RgbEncoder(const std::span<Byte>& buffer, const int bitsPerChannel);

  private:
    std::size_t maxBytesToEncode() const override;
    void encode(const Byte byte) override;

    std::span<Byte> buffer;
    std::span<Byte>::iterator iterator;
    int bitsPerChannel;
};

}
