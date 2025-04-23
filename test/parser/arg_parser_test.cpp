#include "parser/arg_parser.hpp"
#include "parser/config.hpp"
#include "parser/error.hpp"
#include <cstdlib>
#include <gtest/gtest.h>
#include <variant>

namespace whisp
{

namespace
{
const auto filename = "secret_file.txt";
const auto image = "image.png";

template <typename T, std::size_t N>
constexpr std::size_t arraySize(const T (&)[N]) noexcept
{
    return N;
}
}

class ArgParserTestSuite : public testing::Test
{
  protected:
    ArgParser objectUnderTest{};
};

TEST_F(ArgParserTestSuite, Help)
{
    const char* argv[] = {"whisp", "--help"};
    constexpr auto argc = arraySize(argv);

    const auto result = objectUnderTest.parse(argc, argv);
    ASSERT_TRUE(result.has_value());
    ASSERT_FALSE(result->has_value());
}

TEST_F(ArgParserTestSuite, MissingMode)
{
    const char* argv[] = {"whisp", "--image-file", image, "--secret-file", filename};
    constexpr auto argc = arraySize(argv);

    const auto result = objectUnderTest.parse(argc, argv);
    EXPECT_FALSE(result.has_value());
    EXPECT_STREQ(result.error().what(), "A subcommand is required");
}

TEST_F(ArgParserTestSuite, BothModesProvided)
{
    const char* argv[] = {"whisp", "encode", "decode", "--image-file", image, "--secret-file", filename};
    constexpr auto argc = arraySize(argv);

    const auto result = objectUnderTest.parse(argc, argv);
    EXPECT_FALSE(result.has_value());
    EXPECT_STREQ(result.error().what(), "A subcommand is required");
}

TEST_F(ArgParserTestSuite, MissingImageArgumentWhenEncode)
{
    const char* argv[] = {"whisp", "encode", "--secret-file", filename};
    constexpr auto argc = arraySize(argv);

    const auto result = objectUnderTest.parse(argc, argv);
    EXPECT_FALSE(result.has_value());
    EXPECT_STREQ(result.error().what(), "--image-file is required");
}

TEST_F(ArgParserTestSuite, MissingImageArgumentWhenDecode)
{
    const char* argv[] = {"whisp", "decode"};
    constexpr auto argc = arraySize(argv);

    const auto result = objectUnderTest.parse(argc, argv);
    EXPECT_FALSE(result.has_value());
    EXPECT_STREQ(result.error().what(), "--image-file is required");
}

TEST_F(ArgParserTestSuite, MissingFileArgument)
{
    const char* argv[] = {"whisp", "encode", "--image-file", image};
    constexpr auto argc = arraySize(argv);

    const auto result = objectUnderTest.parse(argc, argv);
    EXPECT_FALSE(result.has_value());
    EXPECT_STREQ(result.error().what(), "--secret-file is required");
}

TEST_F(ArgParserTestSuite, MissingAlgorithm)
{
    const char* argv[] = {"whisp", "encode", "--image-file", image, "--secret-file", filename};
    constexpr auto argc = arraySize(argv);

    const auto result = objectUnderTest.parse(argc, argv);
    EXPECT_FALSE(result.has_value());
    EXPECT_STREQ(result.error().what(), "A subcommand is required");
}

TEST_F(ArgParserTestSuite, BitsPerChannelNotSupported)
{
    const auto bitsPerChannel = "3";

    const char* argv[] = {
        "whisp", "encode", "--image-file", image, "--secret-file", filename, "rgb", "--bits", bitsPerChannel};
    constexpr auto argc = arraySize(argv);

    const auto result = objectUnderTest.parse(argc, argv);
    EXPECT_FALSE(result.has_value());
    EXPECT_STREQ(result.error().what(), "--bits: 3 not in {1,2,4}");
}

TEST_F(ArgParserTestSuite, ParseEncodeArgsIntoOptionsAlphaMode)
{
    const char* argv[] = {"whisp", "encode", "--image-file", image, "--secret-file", filename, "alpha"};
    constexpr auto argc = arraySize(argv);

    const auto options = objectUnderTest.parse(argc, argv);
    ASSERT_TRUE(options.has_value());
    ASSERT_TRUE(options.value().has_value());
    EXPECT_TRUE(std::holds_alternative<whisp::EncodeConfig>(options.value().value()));

    const auto& encodeOptions = std::get<whisp::EncodeConfig>(options.value().value());
    EXPECT_EQ(encodeOptions.imageFile, image);
    EXPECT_EQ(encodeOptions.secretFile, filename);
    EXPECT_TRUE(std::holds_alternative<whisp::AlphaAlgorithmConfig>(encodeOptions.algorithmConfig));
}

TEST_F(ArgParserTestSuite, ParseEncodeArgsIntoOptionsRgbMode)
{
    const char* argv[] = {"whisp", "encode", "--image-file", image, "--secret-file", filename, "rgb", "--bits", "4"};
    constexpr auto argc = arraySize(argv);

    const auto options = objectUnderTest.parse(argc, argv);
    ASSERT_TRUE(options.has_value());
    EXPECT_TRUE(std::holds_alternative<whisp::EncodeConfig>(options.value().value()));

    const auto& encodeOptions = std::get<whisp::EncodeConfig>(options.value().value());
    EXPECT_EQ(encodeOptions.imageFile, image);
    EXPECT_EQ(encodeOptions.secretFile, filename);

    EXPECT_TRUE(std::holds_alternative<whisp::RgbAlgorithmConfig>(encodeOptions.algorithmConfig));
    const auto& rgbAlgorithmConfig = std::get<whisp::RgbAlgorithmConfig>(encodeOptions.algorithmConfig);
    EXPECT_EQ(rgbAlgorithmConfig.bitsPerChannel, 4);
}

TEST_F(ArgParserTestSuite, ParseDecodeArgsIntoOptions)
{
    const char* argv[] = {"whisp", "decode", "--image-file", image};
    constexpr auto argc = arraySize(argv);

    const auto options = objectUnderTest.parse(argc, argv);
    ASSERT_TRUE(options.has_value());
    EXPECT_TRUE(std::holds_alternative<whisp::DecodeConfig>(options.value().value()));

    const auto& decodeOptions = std::get<whisp::DecodeConfig>(options.value().value());
    EXPECT_EQ(decodeOptions.imageFile, image);
}

}
