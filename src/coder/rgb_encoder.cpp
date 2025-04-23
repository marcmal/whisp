#include "rgb_encoder.hpp"
#include "coder/coder_data.hpp"
#include "coder/error.hpp"
#include "constants.hpp"
#include "util.hpp"
#include "util/types.hpp"
#include <expected>
#include <spdlog/spdlog.h>

namespace whisp
{

RgbEncoder::RgbEncoder(const std::span<Byte>& data, const int bitsPerChannel)
    : data{data}, bitsPerChannel{bitsPerChannel}, iterator{data.begin()}
{
}

EncodeResult RgbEncoder::encode(const CoderData& dataToEncode)
{
    if (bytesToEncode(dataToEncode.content, dataToEncode.filename) > maxBytesToEncode())
    {
        return std::unexpected(EncodeError{"Too long message to encode."});
    }

    encode(std::span{dataToEncode.filename});
    encode(std::span{dataToEncode.content});
    return {};
}

std::size_t RgbEncoder::bytesToEncode(const std::vector<Byte>& buffer, const std::string& filename) const
{
    return NUM_BYTES_LENGTH_ENCODED + filename.size() + NUM_BYTES_LENGTH_ENCODED + buffer.size();
}

std::size_t RgbEncoder::maxBytesToEncode() const
{
    return (data.size() * bitsPerChannel) / BITS_IN_BYTE;
}

void RgbEncoder::encode(const std::size_t size)
{
    for (auto i = 1; i <= NUM_BYTES_LENGTH_ENCODED; i++)
    {
        encode(static_cast<Byte>(size >> (NUM_BYTES_LENGTH_ENCODED - i) * BITS_IN_BYTE));
    }
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
