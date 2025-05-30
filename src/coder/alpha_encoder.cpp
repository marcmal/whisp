#include "alpha_encoder.hpp"

namespace whisp
{

AlphaEncoder::AlphaEncoder(const std::span<Byte>& buffer)
    : buffer{step_by::makeIterable<4>(buffer | std::views::drop(3))}, cursor{this->buffer.cursor()}
{
}

std::size_t AlphaEncoder::maxBytesToEncode() const
{
    return buffer.size();
}

void AlphaEncoder::encode(const Byte byte)
{
    auto next = cursor.next();
    if (next.has_value())
    {
        **next = byte;
    }
}

}
