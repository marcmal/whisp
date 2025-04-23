#include "alpha_encoder.hpp"
#include "coder/coder_data.hpp"
#include "coder/error.hpp"
#include "constants.hpp"
#include "util/types.hpp"
#include <expected>
#include <spdlog/spdlog.h>

namespace whisp
{

AlphaEncoder::AlphaEncoder(const std::span<Byte>& data) : data{data}, iterator{data.begin()}
{
    iterator += 3;
}

EncodeResult AlphaEncoder::encode(const CoderData& dataToEncode)
{
    if (bytesToEncode(dataToEncode.content, dataToEncode.filename) > maxBytesToEncode())
    {
        return std::unexpected(EncodeError{"Too long message to encode."});
    }

    encode(std::span{dataToEncode.filename});
    encode(std::span{dataToEncode.content});
    return {};
}

std::size_t AlphaEncoder::bytesToEncode(const std::vector<Byte>& buffer, const std::string& filename) const
{
    return NUM_BYTES_LENGTH_ENCODED + filename.size() + NUM_BYTES_LENGTH_ENCODED + buffer.size();
}

std::size_t AlphaEncoder::maxBytesToEncode() const
{
    return data.size() / 4;
}

void AlphaEncoder::encode(const std::size_t size)
{
    for (auto i = 1; i <= NUM_BYTES_LENGTH_ENCODED; i++)
    {
        encode(static_cast<Byte>(size >> (NUM_BYTES_LENGTH_ENCODED - i) * BITS_IN_BYTE));
    }
}

void AlphaEncoder::encode(const Byte byte)
{
    *iterator = byte;
    iterator += 4;
}

}
