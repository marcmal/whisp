#include "parser/arg_parser.hpp"
#include "CLI/CLI.hpp"
#include "parser/config.hpp"
#include "parser/error.hpp"
#include <expected>
#include <iostream>

namespace whisp
{

// NOLINTNEXTLINE(cppcoreguidelines-avoid-c-arrays,modernize-avoid-c-arrays)
Result ArgParser::parse(const int argc, const char* const argv[])
{
    bool verbose{};
    app.add_flag("--verbose", verbose, "Verbose mode")->default_val(false);

    auto* encodeCommand = app.add_subcommand("encode", "Encode an image");
    auto* decodeCommand = app.add_subcommand("decode", "Decode an image");
    app.require_subcommand(1);

    EncodeConfig encodeConfig;
    encodeCommand->add_option("--image-file", encodeConfig.imageFile, "Path to image file to be used to hide data.")
        ->required();
    encodeCommand->add_option("--secret-file", encodeConfig.secretFile, "Path to data file to be hidden.")->required();
    encodeCommand->require_subcommand(1);

    RgbAlgorithmConfig rgbAlgorithmConfig{};
    AlphaAlgorithmConfig alphaAlgorithmConfig{};
    auto* rgbEncodeAlgorithmCommand = encodeCommand->add_subcommand("rgb", "RGB encoding algorithm");
    rgbEncodeAlgorithmCommand
        ->add_option("--bits", rgbAlgorithmConfig.bitsPerChannel, "Number of bits to be used per channel.")
        ->check(CLI::IsMember({1, 2, 4}))
        ->required();
    auto* alphaEncodeAlgorithmCommand = encodeCommand->add_subcommand("alpha", "Alpha encoding algorithm");

    DecodeConfig decodeConfig;
    decodeCommand->add_option("--image-file", decodeConfig.imageFile, "Path to image file holding hidden data.")
        ->required();

    try
    {
        app.parse(argc, argv);
    }
    catch (const CLI::CallForHelp& e)
    {
        return ParserResult{Help{app.help()}, verbose};
    }
    catch (const CLI::ParseError& e)
    {
        return std::unexpected(ParserError{e.what()});
    }

    if (encodeCommand->parsed())
    {
        if (rgbEncodeAlgorithmCommand->parsed())
        {
            encodeConfig.algorithmConfig = rgbAlgorithmConfig;
            return ParserResult{Config{encodeConfig}, verbose};
        }

        if (alphaEncodeAlgorithmCommand->parsed())
        {
            encodeConfig.algorithmConfig = alphaAlgorithmConfig;
            return ParserResult{Config{encodeConfig}, verbose};
        }
    }
    else if (decodeCommand->parsed())
    {
        return ParserResult{Config{decodeConfig}, verbose};
    }

    // Should never happen
    // LCOV_EXCL_START
    std::cout << app.help();
    return std::unexpected(ParserError{"No valid arguments provided!"});
    // LCOV_EXCL_STOP
}

}
