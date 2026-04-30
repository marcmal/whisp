module;

#include <ranges>
#include <span>
#include <vector>

export module whisp.coder:alpha;
import :base;

namespace whisp::coder
{
class AlphaEncoder : public Encoder
{
  public:
    explicit AlphaEncoder(const std::span<Byte>& data);

  private:
    std::size_t maxBytesToEncode() const override;
    void encode(const Byte byte) override;

    using StridedView = decltype(std::declval<std::span<Byte>>() | std::views::drop(3) | std::views::stride(4));
    StridedView view;
    std::ranges::iterator_t<StridedView> it;
};

AlphaEncoder::AlphaEncoder(const std::span<Byte>& buffer)
    : view{buffer | std::views::drop(3) | std::views::stride(BYTES_PER_PIXEL)}, it{std::ranges::begin(view)}
{
}

std::size_t AlphaEncoder::maxBytesToEncode() const
{
    return std::ranges::size(view);
}

void AlphaEncoder::encode(const Byte byte)
{
    if (it != std::ranges::end(view))
    {
        *it++ = byte;
    }
}

class AlphaDecoder : public Decoder
{
  public:
    AlphaDecoder(const std::span<Byte>&);

  private:
    std::size_t decodeLength() override;
    std::vector<Byte> decodeData(std::size_t length) override;
    Byte decodeByte() override;

    using StridedView = decltype(std::declval<std::span<Byte>>() | std::views::drop(3) | std::views::stride(4));
    StridedView view;
    std::ranges::iterator_t<StridedView> it;
};

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