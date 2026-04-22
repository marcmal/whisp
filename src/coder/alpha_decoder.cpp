#include "alpha_decoder.hpp"
#include "coder/coder_data.hpp"
#include "constants.hpp"
#include "exception.hpp"

namespace whisp
{

AlphaDecoder::AlphaDecoder(const std::span<Byte>& data)
    : view{data | std::views::drop(3) | std::views::stride(4)}, it{std::ranges::begin(view)}
{
}

DecodeResult AlphaDecoder::decode()
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

std::size_t AlphaDecoder::decodeLength()
{
    std::size_t size{};

    for (auto i = 1; i <= NUM_BYTES_LENGTH_ENCODED; i++)
    {
        size = (size << BITS_IN_BYTE) | decodeByte();
    }

    return size;
}

std::vector<Byte> AlphaDecoder::decodeData(std::size_t length)
{
    std::vector<Byte> data;
    data.reserve(length);

    while (length-- > 0)
    {
        data.push_back(decodeByte());
    }
    return data;
}

Byte AlphaDecoder::decodeByte()
{
    if (it == std::ranges::end(view))
    {
        throw DecodeException{DecodeError{"Not enough data to decode."}};
    }
    return *it++;
}

}
