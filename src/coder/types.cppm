module;

#include <expected>
#include <spdlog/formatter.h>
#include <string>
#include <vector>

export module whisp.coder:types;

import whisp.util;
import :error;

export namespace whisp::coder
{
using util::Byte;

constexpr auto BITS_IN_BYTE = 8;
constexpr auto NUM_BYTES_LENGTH_ENCODED = 4;
constexpr auto BYTES_PER_PIXEL = 4;

struct CoderData
{
    std::vector<Byte> content;
    std::string secretFileName;
};

using EncodeResult = std::expected<void, EncodeError>;
using DecodeResult = std::expected<CoderData, DecodeError>;
}
