module;

#include <expected>
#include <memory>
#include <span>
#include <variant>

module whisp.coder;

import :header;
import :alpha;
import :rgb;

namespace whisp::coder
{

auto createHeader(const algorithm::Config& algorithmConfig)
{
    return std::visit(
        util::overload{[](const algorithm::AlphaConfig&) { return coder::Header::createAlpha(); },
                       [](const algorithm::RgbConfig& cfg) { return coder::Header::createRgb(cfg.bitsPerChannel); }},
        algorithmConfig);
}

std::unique_ptr<coder::Encoder> createEncoder(const algorithm::Config& algorithmConfig, std::span<Byte> buffer)
{
    auto result = std::visit(util::overload{[&](const algorithm::AlphaConfig&) -> std::unique_ptr<coder::Encoder> {
                                                return std::make_unique<coder::AlphaEncoder>(buffer);
                                            },
                                            [&](const algorithm::RgbConfig& cfg) -> std::unique_ptr<coder::Encoder> {
                                                return std::make_unique<coder::RgbEncoder>(buffer, cfg.bitsPerChannel);
                                            }},
                             algorithmConfig);
    return result;
}

std::unique_ptr<coder::Decoder> createDecoder(const coder::Header& header, std::span<Byte> buffer)
{
    auto result =
        std::visit(util::overload{[&](const coder::AlphaAlgorithmHeader&) -> std::unique_ptr<coder::Decoder> {
                                      return std::make_unique<coder::AlphaDecoder>(buffer);
                                  },
                                  [&](const coder::RgbAlgorithmHeader& config) -> std::unique_ptr<coder::Decoder> {
                                      return std::make_unique<coder::RgbDecoder>(buffer, config.bitsPerChannel);
                                  }},
                   header.header);
    return result;
}

EncodeResult encode(const algorithm::Config& algorithmConfig, std::span<Byte> buffer, const CoderData& coderData)
{
    const auto header = createHeader(algorithmConfig);
    if (buffer.size() < header.size() * BYTES_PER_PIXEL)
    {
        return std::unexpected(EncodeError{"Not enough buffer to encode header."});
    }

    const auto headerBuffer = buffer.subspan(0, header.size() * BYTES_PER_PIXEL);
    const auto dataBuffer = buffer.subspan(header.size() * BYTES_PER_PIXEL);

    encodeHeader(header, headerBuffer);
    return createEncoder(algorithmConfig, dataBuffer)->encode(coderData);
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