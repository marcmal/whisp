module;

#include <spdlog/formatter.h>
#include <variant>

export module whisp.types;

export namespace whisp::algorithm
{
struct RgbConfig
{
    int bitsPerChannel;
};

struct AlphaConfig
{
};

using Config = std::variant<RgbConfig, AlphaConfig>;
}

template <>
struct fmt::formatter<whisp::algorithm::RgbConfig> : fmt::formatter<std::string>
{
    auto format(const whisp::algorithm::RgbConfig& obj, format_context& ctx) const
    {
        return fmt::format_to(ctx.out(), "algorithm::RgbConfig {{ bitsPerChannel: {} }}", obj.bitsPerChannel);
    }
};

template <>
struct fmt::formatter<whisp::algorithm::AlphaConfig> : fmt::formatter<std::string>
{
    auto format(const whisp::algorithm::AlphaConfig&, format_context& ctx) const
    {
        return fmt::format_to(ctx.out(), "algorithm::AlphaConfig {{}}");
    }
};

template <>
struct fmt::formatter<whisp::algorithm::Config> : fmt::formatter<std::string>
{
    auto format(const whisp::algorithm::Config& obj, format_context& ctx) const
    {
        return std::visit([&ctx](const auto& val) { return format_to(ctx.out(), "algorithm::Config {{ {} }}", val); },
                          obj);
    }
};
