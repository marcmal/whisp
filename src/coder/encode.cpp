#include "coder/encode.hpp"
#include "alpha_encoder.hpp"
#include "coder/coder_data.hpp"
#include "coder/encode.hpp"
#include "header.hpp"
#include "header_encoder.hpp"
#include "parser/config.hpp"
#include "rgb_encoder.hpp"
#include "util/types.hpp"
#include <memory>

namespace whisp
{

namespace
{
auto createHeader(const AlgorithmConfig& algorithmConfig)
{
    return std::visit(
        overload{[](const AlphaAlgorithmConfig&) { return Header::createAlpha(); },
                 [](const RgbAlgorithmConfig& config) { return Header::createRgb(config.bitsPerChannel); }},
        algorithmConfig);
}

std::unique_ptr<Encoder> createEncoder(const AlgorithmConfig& algorithmConfig, std::span<Byte> buffer)
{
    auto result = std::visit(overload{[&](const AlphaAlgorithmConfig&) -> std::unique_ptr<Encoder> {
                                          return std::make_unique<AlphaEncoder>(buffer);
                                      },
                                      [&](const RgbAlgorithmConfig& config) -> std::unique_ptr<Encoder> {
                                          return std::make_unique<RgbEncoder>(buffer, config.bitsPerChannel);
                                      }},
                             algorithmConfig);
    return result;
}
}

EncodeResult encode(const AlgorithmConfig& algorithmConfig, std::span<Byte> buffer, const CoderData& coderData)
{
    const auto header = createHeader(algorithmConfig);
    if (buffer.size() < header.size() * 4)
    {
        return std::unexpected(EncodeError{"Not enough buffer to encode header."});
    }

    const auto headerBuffer = buffer.subspan(0, header.size() * 4);
    const auto dataBuffer = buffer.subspan(header.size() * 4);

    encodeHeader(header, headerBuffer).value();
    return createEncoder(algorithmConfig, dataBuffer)->encode(coderData);
}

}