module;

#include <filesystem>
#include <spdlog/formatter.h>
#include <string>
#include <variant>

export module whisp.parser:config;

export namespace whisp::parser
{
struct RgbAlgorithmConfig
{
    int bitsPerChannel;
};

struct AlphaAlgorithmConfig
{
};

using AlgorithmConfig = std::variant<RgbAlgorithmConfig, AlphaAlgorithmConfig>;

struct EncodeConfig
{
    std::filesystem::path imageFile;
    std::filesystem::path secretFile;
    AlgorithmConfig algorithmConfig;
};

struct DecodeConfig
{
    std::filesystem::path imageFile;
};

struct Help
{
    std::string message;
};

using Config = std::variant<EncodeConfig, DecodeConfig, Help>;

struct Result
{
    Config config;
    bool verbose;
};

}

template <>
struct fmt::formatter<whisp::parser::RgbAlgorithmConfig> : fmt::formatter<std::string>
{
    auto format(const whisp::parser::RgbAlgorithmConfig& obj, format_context& ctx) const
    {
        return fmt::format_to(ctx.out(), "RgbAlgorithmConfig {{ bitsPerChannel: {} }}", obj.bitsPerChannel);
    }
};

template <>
struct fmt::formatter<whisp::parser::AlphaAlgorithmConfig> : fmt::formatter<std::string>
{
    auto format(const whisp::parser::AlphaAlgorithmConfig&, format_context& ctx) const
    {
        return fmt::format_to(ctx.out(), "AlphaAlgorithmConfig {{}}");
    }
};

template <>
struct fmt::formatter<whisp::parser::AlgorithmConfig> : fmt::formatter<std::string>
{
    auto format(const whisp::parser::AlgorithmConfig& obj, format_context& ctx) const
    {
        return std::visit([&ctx](const auto& val) { return format_to(ctx.out(), "AlgorithmConfig {{ {} }}", val); },
                          obj);
    }
};

template <>
struct fmt::formatter<whisp::parser::EncodeConfig> : fmt::formatter<std::string>
{
    auto format(const whisp::parser::EncodeConfig& obj, format_context& ctx) const
    {
        return fmt::format_to(ctx.out(),
                              "EncodeConfig {{ imageFile: {}, secretFile: {}, algorithmConfig: {} }}",
                              obj.imageFile.string(),
                              obj.secretFile.string(),
                              obj.algorithmConfig);
    }
};

template <>
struct fmt::formatter<whisp::parser::DecodeConfig> : fmt::formatter<std::string>
{
    auto format(const whisp::parser::DecodeConfig& obj, format_context& ctx) const
    {
        return fmt::format_to(ctx.out(), "DecodeConfig {{ imageFile: {} }}", obj.imageFile.string());
    }
};

template <>
struct fmt::formatter<whisp::parser::Help> : fmt::formatter<std::string>
{
    auto format(const whisp::parser::Help&, format_context& ctx) const
    {
        return fmt::format_to(ctx.out(), "Help {{}}");
    }
};

template <>
struct fmt::formatter<whisp::parser::Config> : fmt::formatter<std::string>
{
    auto format(const whisp::parser::Config& obj, format_context& ctx) const
    {
        return std::visit([&ctx](const auto& val) { return format_to(ctx.out(), "Config {{ {} }}", val); }, obj);
    }
};

template <>
struct fmt::formatter<whisp::parser::Result> : fmt::formatter<std::string>
{
    auto format(const whisp::parser::Result& obj, format_context& ctx) const
    {
        return fmt::format_to(ctx.out(), "Result {{ config: {}, verbose: {} }}", obj.config, obj.verbose);
    }
};
