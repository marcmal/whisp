#include "header_encoder.hpp"
#include "header.hpp"

namespace whisp
{

namespace
{
constexpr auto ALPHA_INDEX = 3;
}

void encodeAlgorithmMode(const int mode, std::span<Byte> buffer)
{
    buffer[ALPHA_INDEX] = mode; // NOLINT(cppcoreguidelines-pro-bounds-avoid-unchecked-container-access)
}

void encodeAlgorithmHeader(const AlgorithmHeader& header, std::span<Byte> buffer)
{
    std::visit(overload{[](const AlphaAlgorithmHeader&) {},
                        [&buffer](const RgbAlgorithmHeader& config) {
                            // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-avoid-unchecked-container-access)
                            buffer[ALPHA_INDEX + 4] = config.bitsPerChannel;
                        }},
               header);
}

void encodeHeader(const Header& header, std::span<Byte> buffer)
{
    encodeAlgorithmMode(header.mode, buffer);
    encodeAlgorithmHeader(header.header, buffer);
}

}
