#include <fmt/format.h>
#include <gtest/gtest.h>

import whisp.parser;

namespace whisp
{

TEST(ConfigTest, FormatEncodeConfig_AlphaAlgorithmConfig)
{
    const auto config = parser::EncodeConfig{"./image_file.png", "./secret_file.txt", parser::AlphaAlgorithmConfig{}};
    const auto result = fmt::format("{}", config);
    EXPECT_EQ(result,
              "EncodeConfig { imageFile: ./image_file.png, secretFile: ./secret_file.txt, algorithmConfig: "
              "AlgorithmConfig { AlphaAlgorithmConfig {} } }");
}

TEST(ConfigTest, FormatEncodeConfig_RgbAlgorithmConfig)
{
    const auto config =
        parser::EncodeConfig{"./image_file.png", "./secret_file.txt", parser::RgbAlgorithmConfig{.bitsPerChannel = 4}};
    const auto result = fmt::format("{}", config);
    EXPECT_EQ(result,
              "EncodeConfig { imageFile: ./image_file.png, secretFile: ./secret_file.txt, algorithmConfig: "
              "AlgorithmConfig { RgbAlgorithmConfig { bitsPerChannel: 4 } } }");
}

TEST(ConfigTest, FormatDecodeConfig)
{
    const auto config = parser::DecodeConfig{"./image_file.png"};
    const auto result = fmt::format("{}", config);
    EXPECT_EQ(result, "DecodeConfig { imageFile: ./image_file.png }");
}

}
