#pragma once

#include <filesystem>
#include <spdlog/formatter.h>
#include <variant>

namespace whisp
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

using Config = std::variant<EncodeConfig, DecodeConfig>;
}

template <>
struct fmt::formatter<whisp::RgbAlgorithmConfig> : fmt::formatter<std::string>
{
    auto format(const whisp::RgbAlgorithmConfig& obj, format_context& ctx) const
    {
        return fmt::format_to(ctx.out(), "RgbAlgorithmConfig {{ bitsPerChannel: {} }}", obj.bitsPerChannel);
    }
};

template <>
struct fmt::formatter<whisp::AlphaAlgorithmConfig> : fmt::formatter<std::string>
{
    auto format(const whisp::AlphaAlgorithmConfig&, format_context& ctx) const
    {
        return fmt::format_to(ctx.out(), "AlphaAlgorithmConfig {{}}");
    }
};

template <>
struct fmt::formatter<whisp::AlgorithmConfig> : fmt::formatter<std::string>
{
    auto format(const whisp::AlgorithmConfig& obj, format_context& ctx) const
    {
        return std::visit([&ctx](const auto& val) { return format_to(ctx.out(), "AlgorithmConfig {{ {} }}", val); },
                          obj);
    }
};

template <>
struct fmt::formatter<whisp::EncodeConfig> : fmt::formatter<std::string>
{
    auto format(const whisp::EncodeConfig& obj, format_context& ctx) const
    {
        return fmt::format_to(ctx.out(),
                              "EncodeConfig {{ imageFile: {}, secretFile: {}, algorithmConfig: {} }}",
                              obj.imageFile.string(),
                              obj.secretFile.string(),
                              obj.algorithmConfig);
    }
};

template <>
struct fmt::formatter<whisp::DecodeConfig> : fmt::formatter<std::string>
{
    auto format(const whisp::DecodeConfig& obj, format_context& ctx) const
    {
        return fmt::format_to(ctx.out(), "DecodeConfig {{ imageFile: {} }}", obj.imageFile.string());
    }
};
