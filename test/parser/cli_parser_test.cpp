#include "parser/argument_exception.hpp"
#include "parser/cli_parser.hpp"
#include "gmock/gmock.h"
#include <cstdlib>
#include <gtest/gtest.h>
#include <variant>

namespace fict_tele
{

namespace
{
const auto filename = "secret_file.txt";
const auto image = "image.png";
const auto bitsPerChannel = "3";
}

class CliParserTestSuite : public testing::Test
{
  protected:
    CliParser objectUnderTest{};
};

TEST_F(CliParserTestSuite, Help)
{
    const auto argc = 2;
    const char* argv[] = {"fictional_telegram", "--help"};

    EXPECT_EXIT(objectUnderTest.parse(argc, argv), testing::ExitedWithCode(EXIT_SUCCESS), "");
}

TEST_F(CliParserTestSuite, MissingCodeMode)
{
    const auto argc = 5;
    const char* argv[] = {"fictional_telegram", "--image", image, "--file", filename};

    EXPECT_THAT([&]() { objectUnderTest.parse(argc, argv); },
                testing::Throws<ArgumentException>(testing::Property(
                    &ArgumentException::what, testing::HasSubstr("Error: --encode or --decode option required."))));
}

TEST_F(CliParserTestSuite, BothModesProvided)
{
    const auto argc = 7;
    const char* argv[] = {"fictional_telegram", "--encode", "--decode", "--image", image, "--file", filename};

    EXPECT_THAT([&]() { objectUnderTest.parse(argc, argv); },
                testing::Throws<ArgumentException>(testing::Property(
                    &ArgumentException::what, testing::HasSubstr("Error: --encode or --decode option required."))));
}

TEST_F(CliParserTestSuite, MissingImageArgument)
{
    const auto argc = 4;
    const char* argv[] = {"fictional_telegram", "--encode", "--file", filename};

    EXPECT_THAT([&]() { objectUnderTest.parse(argc, argv); },
                testing::Throws<ArgumentException>(testing::Property(
                    &ArgumentException::what, testing::HasSubstr("Error: missing 'image' argument."))));
}

TEST_F(CliParserTestSuite, MissingFileArgument)
{
    const auto argc = 4;
    const char* argv[] = {"fictional_telegram", "--encode", "--image", image};

    EXPECT_THAT([&]() { objectUnderTest.parse(argc, argv); },
                testing::Throws<ArgumentException>(testing::Property(
                    &ArgumentException::what, testing::HasSubstr("Error: missing 'file' argument."))));
}

TEST_F(CliParserTestSuite, BitsPerChannelNotSupported)
{
    const auto argc = 8;
    const char* argv[] = {"fictional_telegram", "--encode", "--image", image, "--file", filename, "--bits",
                          bitsPerChannel};

    EXPECT_THAT([&]() { objectUnderTest.parse(argc, argv); },
                testing::Throws<ArgumentException>(testing::Property(
                    &ArgumentException::what, testing::HasSubstr("Error: --bits support values are: 1/2/4"))));
}

TEST_F(CliParserTestSuite, ParseEncodeArgsIntoOptions)
{
    const auto argc = 6;
    const char* argv[] = {"fictional_telegram", "--encode", "--image", image, "--file", filename};

    const auto options = objectUnderTest.parse(argc, argv);
    EXPECT_TRUE(std::holds_alternative<config::Encode>(options));

    const auto& encodeOptions = std::get<config::Encode>(options);
    EXPECT_EQ(encodeOptions.imageFilename, image);
    EXPECT_EQ(encodeOptions.fileToEncode, filename);
}

TEST_F(CliParserTestSuite, ParseDecodeArgsIntoOptions)
{
    const auto argc = 4;
    const char* argv[] = {"fictional_telegram", "--decode", "--image", image};

    const auto options = objectUnderTest.parse(argc, argv);
    EXPECT_TRUE(std::holds_alternative<config::Decode>(options));

    const auto& decodeOptions = std::get<config::Decode>(options);
    EXPECT_EQ(decodeOptions.imageFileName, image);
}

}
