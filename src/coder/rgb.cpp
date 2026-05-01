module;

#include <span>
#include <vector>

module whisp.coder:rgb;
import :base;
import :util;

namespace whisp::coder
{
class RgbEncoder : public Encoder
{
  public:
    RgbEncoder(const std::span<Byte>& buffer, const int bitsPerChannel);

  private:
    std::size_t maxBytesToEncode() const override;
    void encode(const Byte byte) override;

    std::span<Byte> buffer;
    std::span<Byte>::iterator iterator;
    int bitsPerChannel;
};

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

class RgbDecoder : public Decoder
{
  public:
    RgbDecoder(const std::span<Byte>& data, const int bitsPerChannel);

  private:
    std::size_t decodeLength() override;
    std::vector<Byte> decodeData(std::size_t length) override;
    Byte decodeByte() override;

    std::span<Byte> buffer;
    std::span<Byte>::iterator iterator;
    int bitsPerChannel;
};

RgbDecoder::RgbDecoder(const std::span<Byte>& buffer, const int bitsPerChannel)
    : buffer{buffer}, iterator{buffer.begin()}, bitsPerChannel{bitsPerChannel}
{
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