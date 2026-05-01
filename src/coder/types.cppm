module;

#include <expected>
#include <spdlog/formatter.h>

import whisp.util;

export module whisp.coder:types;

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

struct EncodeError : std::runtime_error
{
    using std::runtime_error::runtime_error;
};

struct DecodeError : std::runtime_error
{
    using std::runtime_error::runtime_error;
};

struct DecodeException
{
    DecodeError cause;
};

struct EncodeException
{
    EncodeError cause;
};

using EncodeResult = std::expected<void, EncodeError>;
using DecodeResult = std::expected<CoderData, DecodeError>;
}

template <>
struct fmt::formatter<whisp::coder::EncodeError> : fmt::formatter<std::string_view>
{
    auto format(const whisp::coder::EncodeError& err, format_context& ctx) const -> decltype(ctx.out())
    {
        return fmt::format_to(ctx.out(), "[EncodeError: {}]", err.what());
    }
};

template <>
struct fmt::formatter<whisp::coder::DecodeError> : fmt::formatter<std::string_view>
{
    auto format(const whisp::coder::DecodeError& err, format_context& ctx) const -> decltype(ctx.out())
    {
        return fmt::format_to(ctx.out(), "[DecodeError: {}]", err.what());
    }
};