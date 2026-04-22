#include "alpha_encoder.hpp"

namespace whisp
{

AlphaEncoder::AlphaEncoder(const std::span<Byte>& buffer)
    : view{buffer | std::views::drop(3) | std::views::stride(4)}, it{std::ranges::begin(view)}
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

}
