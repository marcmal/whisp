#include "coder/decoder.hpp"
#include "coder/defs.hpp"
#include "coder/util.hpp"

namespace fict_tele
{

Decoder::Decoder(const std::span<Byte>& data, const int bitsPerChannel)
    : data{data}, bitsPerChannel{bitsPerChannel}, iterator{data.begin()}
{
}

std::vector<Byte> Decoder::decode()
{
    auto size = decodeSize();

    std::vector<Byte> message;
    message.reserve(size);

    while (size-- > 0)
    {
        const auto byte = decodeByte();
        message.push_back(byte);
    }

    return message;
}

std::size_t Decoder::decodeSize()
{
    std::size_t size{};

    for (auto i = 1; i <= NUM_BYTES_MSG_LENGTH; i++)
    {
        size = (size << 8) | decodeByte();
    }

    return size;
}

Byte Decoder::decodeByte()
{
    const auto mask = createMask(bitsPerChannel);
    Byte byte{};

    auto bitsLeft = BITS_IN_BYTE;
    while (bitsLeft > 0)
    {
        bitsLeft -= bitsPerChannel;

        const auto bits = *iterator++ & mask;
        byte <<= bitsPerChannel;
        byte |= bits;
    }
    return byte;
}

}
