module;

#include <spdlog/formatter.h>

export module whisp.coder:error;

export namespace whisp::coder
{

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