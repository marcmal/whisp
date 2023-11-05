#include "parser/cli_parser.hpp"
#include "parser/argument_exception.hpp"
#include "parser/config.hpp"
#include <iostream>

namespace fict_tele
{
CliParser::CliParser() : options{"Fictional Telegram", ""}
{
    // clang-format off
  options.add_options()
    ("e,encode", "Encoding mode")
    ("d,decode", "Decoding mode")
    ("i,image", "Image that hides message", cxxopts::value<std::filesystem::path>())
    ("f,file", "File to hide", cxxopts::value<std::filesystem::path>())
    ("b,bits", "Num of bits per channel to encode/decode", cxxopts::value<int>()->default_value("2"))
    ("h,help", "Print usage");
    // clang-format on
}

Config CliParser::parse(const int argc, const char* const argv[])
{
    const auto result = options.parse(argc, argv);

    if (result.count("help"))
    {
        std::cout << options.help() << std::endl;
        exit(EXIT_SUCCESS);
    }

    validateArgs(result);
    return parse(result);
}

void CliParser::validateArgs(const cxxopts::ParseResult& parseResult) const
{
    const auto encodeArgCount = parseResult.count("encode");
    const auto decodeArgCount = parseResult.count("decode");
    const auto msgArgCount = parseResult.count("file");
    const auto bitsPerChannel = parseResult["bits"].as<int>();

    if ((not encodeArgCount and not decodeArgCount) or (encodeArgCount and decodeArgCount))
    {
        throw ArgumentException{"Error: --encode or --decode option required.\n" + options.help()};
    }

    if (not parseResult.count("image"))
    {
        throw ArgumentException{"Error: missing 'image' argument.\n" + options.help()};
    }

    if (encodeArgCount and not msgArgCount)
    {
        throw ArgumentException{"Error: missing 'file' argument.\n" + options.help()};
    }

    if (bitsPerChannel != 1 and bitsPerChannel != 2 and bitsPerChannel != 4)
    {
        throw ArgumentException{"Error: --bits support values are: 1/2/4"};
    }
}

Config CliParser::parse(const cxxopts::ParseResult& parseResult) const
{
    const auto image = parseResult["image"].as<std::filesystem::path>();
    const auto bitsPerChannel = parseResult["bits"].as<int>();

    const auto encodeArgCount = parseResult.count("encode");
    if (encodeArgCount)
    {
        const auto filename = parseResult["file"].as<std::filesystem::path>();
        return config::Encode{image, filename, bitsPerChannel};
    }
    else
    {
        return config::Decode{image, bitsPerChannel};
    }
}

}
