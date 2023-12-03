#include "coder/encoder.hpp"
#include "coder/defs.hpp"
#include "coder/encode_exception.hpp"
#include "coder/util.hpp"
#include "util/types.hpp"
#include <iostream>

namespace fict_tele
{

Encoder::Encoder(const std::span<Byte>& data, const int bitsPerChannel)
    : data{data}, bitsPerChannel{bitsPerChannel}, iterator{data.begin()}
{
}

void Encoder::encode(const std::vector<Byte>& buffer, const std::string& filename)
{
    if (bytesToEncode(buffer, filename) > maxBytesToEncode())
    {
        throw EncodeException{"Message too long to encode in image."};
    }

    encode(std::span{filename});
    encode(std::span{buffer});
}

std::size_t Encoder::bytesToEncode(const std::vector<Byte>& buffer, const std::string& filename) const
{
    return NUM_BYTES_LENGTH_ENCODED + filename.size() + NUM_BYTES_LENGTH_ENCODED + buffer.size();
}

std::size_t Encoder::maxBytesToEncode() const
{
    return (data.size() * bitsPerChannel) / BITS_IN_BYTE;
}

void Encoder::encode(const std::size_t size)
{
    for (auto i = 1; i <= NUM_BYTES_LENGTH_ENCODED; i++)
    {
        encode(static_cast<Byte>(size >> (NUM_BYTES_LENGTH_ENCODED - i) * BITS_IN_BYTE));
    }
}

void Encoder::encode(const Byte byte)
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
