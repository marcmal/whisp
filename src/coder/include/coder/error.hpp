#pragma once

#include <spdlog/formatter.h>

namespace whisp
{

struct EncodeError : std::runtime_error
{
    using std::runtime_error::runtime_error;
};

struct DecodeError : std::runtime_error
{
    using std::runtime_error::runtime_error;
};

}

template <>
struct fmt::formatter<whisp::EncodeError> : fmt::formatter<std::string_view>
{
    auto format(const whisp::EncodeError& err, format_context& ctx) const -> decltype(ctx.out())
    {
        return fmt::format_to(ctx.out(), "[EncodeError: {}]", err.what());
    }
};

template <>
struct fmt::formatter<whisp::DecodeError> : fmt::formatter<std::string_view>
{
    auto format(const whisp::DecodeError& err, format_context& ctx) const -> decltype(ctx.out())
    {
        return fmt::format_to(ctx.out(), "[DecodeError: {}]", err.what());
    }
};