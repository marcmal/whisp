#include "coder/encoder.hpp"
#include "coder/defs.hpp"
#include "coder/encode_exception.hpp"
#include "coder/util.hpp"
#include "util/types.hpp"

namespace fict_tele
{

Encoder::Encoder(const std::span<Byte>& data, const int bitsPerChannel)
    : data{data}, bitsPerChannel{bitsPerChannel}, iterator{data.begin()}
{
}

void Encoder::encode(const std::vector<Byte>& buffer)
{
    const auto maxBytesToEncode = (data.size() * bitsPerChannel) / BITS_IN_BYTE;
    const auto messageLength = buffer.size();

    if ((messageLength + NUM_BYTES_MSG_LENGTH) > maxBytesToEncode)
    {
        throw EncodeException{"Message too long to encode in image."};
    }

    encodeSize(messageLength);

    for (const auto c : buffer)
    {
        encodeByte(c);
    }
}

void Encoder::encodeSize(const std::size_t size)
{
    for (auto i = 1; i <= NUM_BYTES_MSG_LENGTH; i++)
    {
        encodeByte(static_cast<Byte>(size >> (NUM_BYTES_MSG_LENGTH - i) * BITS_IN_BYTE));
    }
}

void Encoder::encodeByte(const Byte byte)
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
