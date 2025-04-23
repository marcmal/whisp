#include "header_encoder.hpp"
#include "header.hpp"
#include <expected>

namespace whisp
{

namespace
{
constexpr auto ALPHA_INDEX = 3;
}

void encodeAlgorithmMode(const int mode, std::span<Byte> buffer)
{
    buffer[ALPHA_INDEX] = mode;
}

void encodeAlgorithmHeader(const AlgorithmHeader& header, std::span<Byte> buffer)
{
    std::visit(
        overload{[](const AlphaAlgorithmHeader&) {},
                 [&buffer](const RgbAlgorithmHeader& config) { buffer[ALPHA_INDEX + 4] = config.bitsPerChannel; }},
        header);
}

EncodeResult encodeHeader(const Header& header, std::span<Byte> buffer)
{
    encodeAlgorithmMode(header.mode, buffer);
    encodeAlgorithmHeader(header.header, buffer);
    return {};
}

}