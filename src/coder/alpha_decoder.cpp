#include "alpha_decoder.hpp"
#include "constants.hpp"
#include "exception.hpp"

namespace whisp
{

AlphaDecoder::AlphaDecoder(const std::span<Byte>& data)
    : view{data | std::views::drop(3) | std::views::stride(BYTES_PER_PIXEL)}, it{std::ranges::begin(view)}
{
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
