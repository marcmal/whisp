module;

#include <spdlog/formatter.h>

export module whisp.parser:error;

export namespace whisp::parser
{
struct ParserError : std::runtime_error
{
    using std::runtime_error::runtime_error;
};
}

template <>
struct fmt::formatter<whisp::parser::ParserError> : fmt::formatter<std::string_view>
{
    auto format(const whisp::parser::ParserError& err, format_context& ctx) const -> decltype(ctx.out())
    {
        return fmt::format_to(ctx.out(), "[ParserError: {}]", err.what());
    }
};
