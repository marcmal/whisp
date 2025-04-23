#pragma once

#include <spdlog/formatter.h>

namespace whisp
{

struct ParserError : std::runtime_error
{
    using std::runtime_error::runtime_error;
};

}

template <>
struct fmt::formatter<whisp::ParserError> : fmt::formatter<std::string_view>
{
    auto format(const whisp::ParserError& err, format_context& ctx) const -> decltype(ctx.out())
    {
        return fmt::format_to(ctx.out(), "[ParserError: {}]", err.what());
    }
};