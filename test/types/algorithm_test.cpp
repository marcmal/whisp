#include <fmt/format.h>
#include <gtest/gtest.h>

import whisp.types;

namespace whisp
{

TEST(TypesAlgorithm, FormatAlphaConfig)
{
    const auto config = algorithm::AlphaConfig{};
    const auto result = fmt::format("{}", config);
    EXPECT_EQ(result, "algorithm::AlphaConfig {}");
}

TEST(TypesAlgorithm, FormatRgbConfig)
{
    const auto config = algorithm::RgbConfig{.bitsPerChannel = 4};
    const auto result = fmt::format("{}", config);
    EXPECT_EQ(result, "algorithm::RgbConfig { bitsPerChannel: 4 }");
}

TEST(TypesAlgorithm, FormatConfig)
{
    const auto config = algorithm::Config{algorithm::RgbConfig{.bitsPerChannel = 4}};
    const auto result = fmt::format("{}", config);
    EXPECT_EQ(result, "algorithm::Config { algorithm::RgbConfig { bitsPerChannel: 4 } }");
}

}
