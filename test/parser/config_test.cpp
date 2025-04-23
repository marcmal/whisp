#include "parser/config.hpp"
#include <fmt/format.h>
#include <gtest/gtest.h>

namespace whisp
{

TEST(ConfigTest, FormatEncodeConfig_AlphaAlgorithmConfig)
{
    const EncodeConfig config = EncodeConfig{"./image_file.png", "./secret_file.txt", AlphaAlgorithmConfig{}};
    const auto result = fmt::format("{}", config);
    EXPECT_EQ(result,
              "EncodeConfig { imageFile: ./image_file.png, secretFile: ./secret_file.txt, algorithmConfig: "
              "AlgorithmConfig { AlphaAlgorithmConfig {} } }");
}

TEST(ConfigTest, FormatEncodeConfig_RgbAlgorithmConfig)
{
    const EncodeConfig config =
        EncodeConfig{"./image_file.png", "./secret_file.txt", RgbAlgorithmConfig{.bitsPerChannel = 4}};
    const auto result = fmt::format("{}", config);
    EXPECT_EQ(result,
              "EncodeConfig { imageFile: ./image_file.png, secretFile: ./secret_file.txt, algorithmConfig: "
              "AlgorithmConfig { RgbAlgorithmConfig { bitsPerChannel: 4 } } }");
}

TEST(ConfigTest, FormatDecodeConfig)
{
    const DecodeConfig config = DecodeConfig{"./image_file.png"};
    const auto result = fmt::format("{}", config);
    EXPECT_EQ(result, "DecodeConfig { imageFile: ./image_file.png }");
}

}
