#include "rgb_encoder.hpp"
#include "util.hpp"

namespace whisp
{

RgbEncoder::RgbEncoder(const std::span<Byte>& buffer, const int bitsPerChannel)
    : buffer{buffer}, iterator{buffer.begin()}, bitsPerChannel{bitsPerChannel}
{
}

std::size_t RgbEncoder::maxBytesToEncode() const
{
    return (buffer.size() * bitsPerChannel) / BITS_IN_BYTE;
}

void RgbEncoder::encode(const Byte byte)
{
    const auto mask = createMask(bitsPerChannel);

    int shift = BITS_IN_BYTE - bitsPerChannel;
    while (shift >= 0)
    {
        const auto bits = (byte >> shift) & mask;
        (*iterator) = (*iterator & ~mask) | bits;
        ++iterator;
        shift -= bitsPerChannel;
    }
}

}
