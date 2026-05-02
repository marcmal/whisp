#include <fmt/format.h>
#include <gtest/gtest.h>

import whisp.parser;

namespace whisp
{

TEST(ConfigTest, FormatEncodeConfig_AlphaConfig)
{
    const auto config = parser::EncodeConfig{"./image_file.png", "./secret_file.txt", algorithm::AlphaConfig{}};
    const auto result = fmt::format("{}", config);
    EXPECT_EQ(result,
              "EncodeConfig { imageFile: ./image_file.png, secretFile: ./secret_file.txt, algorithmConfig: "
              "algorithm::Config { algorithm::AlphaConfig {} } }");
}

TEST(ConfigTest, FormatEncodeConfig_RgbConfig)
{
    const auto config =
        parser::EncodeConfig{"./image_file.png", "./secret_file.txt", algorithm::RgbConfig{.bitsPerChannel = 4}};
    const auto result = fmt::format("{}", config);
    EXPECT_EQ(result,
              "EncodeConfig { imageFile: ./image_file.png, secretFile: ./secret_file.txt, algorithmConfig: "
              "algorithm::Config { algorithm::RgbConfig { bitsPerChannel: 4 } } }");
}

TEST(ConfigTest, FormatDecodeConfig)
{
    const auto config = parser::DecodeConfig{"./image_file.png"};
    const auto result = fmt::format("{}", config);
    EXPECT_EQ(result, "DecodeConfig { imageFile: ./image_file.png }");
}

TEST(ConfigTest, FormatHelp)
{
    const auto config = parser::Help{"./image_file.png"};
    const auto result = fmt::format("{}", config);
    EXPECT_EQ(result, "Help {}");
}

TEST(ConfigTest, FormatConfig)
{
    const auto config = parser::Config{parser::DecodeConfig{"./image_file.png"}};
    const auto result = fmt::format("{}", config);
    EXPECT_EQ(result, "Config { DecodeConfig { imageFile: ./image_file.png } }");
}

TEST(ConfigTest, FormatResult)
{
    constexpr auto verbose = true;
    const auto config = parser::Result{parser::DecodeConfig{"./img.png"}, verbose};
    const auto result = fmt::format("{}", config);
    EXPECT_EQ(result, "Result { config: Config { DecodeConfig { imageFile: ./img.png } }, verbose: true }");
}

}
