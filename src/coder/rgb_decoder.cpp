#include "rgb_decoder.hpp"
#include "coder/coder_data.hpp"
#include "coder/error.hpp"
#include "constants.hpp"
#include "exception.hpp"
#include "util.hpp"

namespace whisp
{

RgbDecoder::RgbDecoder(const std::span<Byte>& buffer, const int bitsPerChannel)
    : buffer{buffer}, iterator{buffer.begin()}, bitsPerChannel{bitsPerChannel}
{
}

DecodeResult RgbDecoder::decode()
{
    try
    {
        const auto filenameLength = decodeLength();
        const auto filename = decodeData(filenameLength);
        const auto dataLength = decodeLength();
        const auto decodedData = decodeData(dataLength);
        return CoderData{decodedData, std::string{filename.begin(), filename.end()}};
    }
    catch (const DecodeException& e)
    {
        return std::unexpected(e.cause);
    }
}

std::size_t RgbDecoder::decodeLength()
{
    std::size_t size{};

    for (auto i = 1; i <= NUM_BYTES_LENGTH_ENCODED; i++)
    {
        size = (size << BITS_IN_BYTE) | decodeByte();
    }

    return size;
}

std::vector<Byte> RgbDecoder::decodeData(std::size_t length)
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

Byte RgbDecoder::decodeByte()
{
    const auto mask = createMask(bitsPerChannel);
    Byte byte{};

    auto bitsLeft = BITS_IN_BYTE;
    while (bitsLeft > 0)
    {
        bitsLeft -= bitsPerChannel;

        const auto next = iterator++;
        if (next == buffer.end())
        {
            throw DecodeException{DecodeError{"Not enough data to decode."}};
        }

        const auto bits = *next & mask;
        byte <<= bitsPerChannel;
        byte |= bits;
    }
    return byte;
}
}
