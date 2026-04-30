module;

#include <expected>
#include <format>
#include <span>
#include <variant>

export module whisp.coder:header;
import :util;
import :types;

namespace
{
constexpr auto ALPHA_INDEX = 3;
}

namespace whisp::coder
{

struct AlphaAlgorithmHeader
{
};

struct RgbAlgorithmHeader
{
    int bitsPerChannel;
};

using AlgorithmHeader = std::variant<AlphaAlgorithmHeader, RgbAlgorithmHeader>;

struct Header
{
    constexpr static auto ALPHA_MODE = 1;
    constexpr static auto RGB_MODE = 2;

    static auto createAlpha()
    {
        return Header{ALPHA_MODE, AlphaAlgorithmHeader{}};
    }

    static auto createRgb(const int bitsPerChannel)
    {
        return Header{RGB_MODE, RgbAlgorithmHeader{bitsPerChannel}};
    }

    std::size_t size() const
    {
        return std::visit(overload{
                              [](const AlphaAlgorithmHeader&) { return 1; },
                              [](const RgbAlgorithmHeader&) { return 2; },
                          },
                          header);
    }

    int mode;
    AlgorithmHeader header;
};

void encodeAlgorithmMode(const int mode, std::span<Byte> buffer)
{
    buffer[ALPHA_INDEX] = mode; // NOLINT(cppcoreguidelines-pro-bounds-avoid-unchecked-container-access)
}

void encodeAlgorithmHeader(const AlgorithmHeader& header, std::span<Byte> buffer)
{
    std::visit(overload{[](const AlphaAlgorithmHeader&) {},
                        [&buffer](const RgbAlgorithmHeader& config) {
                            // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-avoid-unchecked-container-access)
                            buffer[ALPHA_INDEX + BYTES_PER_PIXEL] = config.bitsPerChannel;
                        }},
               header);
}

void encodeHeader(const Header& header, std::span<Byte> buffer)
{
    encodeAlgorithmMode(header.mode, buffer);
    encodeAlgorithmHeader(header.header, buffer);
}

std::expected<AlgorithmHeader, DecodeError> decodeAlgorithmHeader(const int mode, const std::span<Byte>& data)
{
    if (mode == Header::ALPHA_MODE)
    {
        return AlphaAlgorithmHeader{};
    }
    if (mode == Header::RGB_MODE)
    {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-avoid-unchecked-container-access)
        return RgbAlgorithmHeader{data[3 + BYTES_PER_PIXEL]};
    }
    return std::unexpected(DecodeError{std::format("Decoding algorithm header failure, mode: {}", mode)});
}

std::expected<Header, DecodeError> decodeHeader(const std::span<Byte>& data)
{
    const auto mode = data[3]; // NOLINT(cppcoreguidelines-pro-bounds-avoid-unchecked-container-access)
    const auto algorithmHeader = decodeAlgorithmHeader(mode, data);
    if (not algorithmHeader.has_value())
    {
        return std::unexpected(algorithmHeader.error());
    }
    return Header{mode, algorithmHeader.value()};
}
}