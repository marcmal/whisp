#include "coder/decode.hpp"
#include "alpha_decoder.hpp"
#include "constants.hpp"
#include "decoder.hpp"
#include "header.hpp"
#include "header_decoder.hpp"
#include "rgb_decoder.hpp"

namespace whisp
{

namespace
{
std::unique_ptr<Decoder> createDecoder(const Header& header, std::span<Byte> buffer)
{
    auto result = std::visit(overload{[&](const AlphaAlgorithmHeader&) -> std::unique_ptr<Decoder> {
                                          return std::make_unique<AlphaDecoder>(buffer);
                                      },
                                      [&](const RgbAlgorithmHeader& config) -> std::unique_ptr<Decoder> {
                                          return std::make_unique<RgbDecoder>(buffer, config.bitsPerChannel);
                                      }},
                             header.header);
    return result;
}
}

DecodeResult decode(const std::span<Byte>& buffer)
{
    const auto header = decodeHeader(buffer);
    if (not header.has_value())
    {
        return std::unexpected(header.error());
    }
    auto decoder = createDecoder(header.value(), buffer.subspan(header.value().size() * BYTES_PER_PIXEL));
    return decoder->decode();
}

}
