#include "coder/error.hpp"
#include "constants.hpp"
#include "header.hpp"
#include "header_encoder.hpp"
#include <expected>
#include <format>

namespace whisp
{

namespace
{

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
