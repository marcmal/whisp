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

auto createHeader(const parser::AlgorithmConfig& algorithmConfig)
{
    return std::visit(overload{[](const parser::AlphaAlgorithmConfig&) { return coder::Header::createAlpha(); },
                               [](const parser::RgbAlgorithmConfig& config) {
                                   return coder::Header::createRgb(config.bitsPerChannel);
                               }},
                      algorithmConfig);
}

std::unique_ptr<coder::Encoder> createEncoder(const parser::AlgorithmConfig& algorithmConfig, std::span<Byte> buffer)
{
    auto result = std::visit(overload{[&](const parser::AlphaAlgorithmConfig&) -> std::unique_ptr<coder::Encoder> {
                                          return std::make_unique<coder::AlphaEncoder>(buffer);
                                      },
                                      [&](const parser::RgbAlgorithmConfig& config) -> std::unique_ptr<coder::Encoder> {
                                          return std::make_unique<coder::RgbEncoder>(buffer, config.bitsPerChannel);
                                      }},
                             algorithmConfig);
    return result;
}

std::unique_ptr<coder::Decoder> createDecoder(const coder::Header& header, std::span<Byte> buffer)
{
    auto result = std::visit(overload{[&](const coder::AlphaAlgorithmHeader&) -> std::unique_ptr<coder::Decoder> {
                                          return std::make_unique<coder::AlphaDecoder>(buffer);
                                      },
                                      [&](const coder::RgbAlgorithmHeader& config) -> std::unique_ptr<coder::Decoder> {
                                          return std::make_unique<coder::RgbDecoder>(buffer, config.bitsPerChannel);
                                      }},
                             header.header);
    return result;
}

EncodeResult encode(const parser::AlgorithmConfig& algorithmConfig, std::span<Byte> buffer, const CoderData& coderData)
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