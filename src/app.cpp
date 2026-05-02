module;

#include <iostream>
#include <spdlog/common.h>
#include <spdlog/spdlog.h>
#include <variant>

module whisp.app;

import whisp.io;
import whisp.util;
import whisp.coder;

namespace whisp
{

// NOLINTNEXTLINE(cppcoreguidelines-avoid-c-arrays,modernize-avoid-c-arrays)
int App::run(const int argc, const char* const argv[])
{
    try
    {
        const auto options = parser::ArgParser{}.parse(argc, argv);

        if (options.has_value())
        {
            if (options->verbose)
            {
                spdlog::set_level(spdlog::level::debug);
            }
            return std::visit(util::overload{[this](const parser::EncodeConfig& config) { return encode(config); },
                                             [this](const parser::DecodeConfig& config) { return decode(config); },
                                             [](const parser::Help& help) {
                                                 std::cout << help.message;
                                                 return EXIT_SUCCESS;
                                             }},
                              options->config);
        }
        else
        {
            spdlog::error("{}", options.error());
            return EXIT_FAILURE;
        }
    }
    // LCOV_EXCL_START
    catch (const std::exception& e)
    {
        spdlog::error("Unknown exception: {}", e.what());
        return EXIT_FAILURE;
    }
    // LCOV_EXCL_STOP
}

int App::encode(const parser::EncodeConfig& config)
{
    spdlog::info("Running encode with config: {}", config);

    auto imageData = io::readImage(config.imageFile);

    io::FileReader fileReader{config.secretFile};
    coder::CoderData dataToEncode{fileReader.read(), config.secretFile};
    const auto encodeResult = coder::encode(config.algorithmConfig, std::span{imageData.pixels}, dataToEncode);

    if (encodeResult.has_value())
    {
        const auto outputPath = config.imageFile.parent_path() /= ("encoded_" + config.imageFile.filename().string());
        io::saveImage(imageData, outputPath);

        return EXIT_SUCCESS;
    }
    else
    {
        spdlog::error("{}", encodeResult.error());
        return EXIT_FAILURE;
    }
}

int App::decode(const parser::DecodeConfig& config)
{
    spdlog::info("Running decode with config: {}", config);

    auto image = io::readImage(config.imageFile);
    std::span imageData{image.pixels};

    const auto decodeResult = coder::decode(imageData);
    if (decodeResult.has_value())
    {
        const auto [data, filename] = decodeResult.value();
        const auto outputPath = config.imageFile.parent_path() /= filename;
        io::FileWriter fileWriter{outputPath};
        fileWriter.write(data);
        return EXIT_SUCCESS;
    }
    else
    {
        spdlog::error("{}", decodeResult.error());
        return EXIT_FAILURE;
    }
}
}