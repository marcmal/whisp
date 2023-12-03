#include "coder/decoder.hpp"
#include "coder/defs.hpp"
#include "coder/util.hpp"
#include <iostream>

namespace fict_tele
{

Decoder::Decoder(const std::span<Byte>& data, const int bitsPerChannel)
    : data{data}, bitsPerChannel{bitsPerChannel}, iterator{data.begin()}
{
}

std::pair<std::string, std::vector<Byte>> Decoder::decode()
{
    const auto filename = decodeData(decodeLength());
    const auto data = decodeData(decodeLength());
    return {{filename.begin(), filename.end()}, data};
}

std::size_t Decoder::decodeLength()
{
    std::size_t size{};

    for (auto i = 1; i <= NUM_BYTES_LENGTH_ENCODED; i++)
    {
        size = (size << BITS_IN_BYTE) | decodeByte();
    }

    return size;
}

std::vector<Byte> Decoder::decodeData(std::size_t length)
{
    std::vector<Byte> data;
    data.reserve(length);

    while (length-- > 0)
    {
        const auto byte = decodeByte();
        data.push_back(byte);
    }

    return data;
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
