#include "app.hpp"
#include "coder/coder_data.hpp"
#include "coder/decode.hpp"
#include "coder/encode.hpp"
#include "io/file_reader.hpp"
#include "io/file_writer.hpp"
#include "io/image_reader.hpp"
#include "parser/arg_parser.hpp"
#include "parser/config.hpp"
#include "util/overload.hpp"
#include <cstdlib>
#include <spdlog/spdlog.h>

namespace whisp
{

// NOLINTNEXTLINE(cppcoreguidelines-avoid-c-arrays,modernize-avoid-c-arrays)
int App::run(const int argc, const char* const argv[])
{
    try
    {
        const auto options = whisp::ArgParser{}.parse(argc, argv);
        if (options.has_value())
        {
            if (const auto& optionsResult = options.value(); optionsResult.has_value())
            {
                return std::visit(overload{[this](const whisp::EncodeConfig& config) { return encode(config); },
                                           [this](const whisp::DecodeConfig& config) { return decode(config); }},
                                  optionsResult.value());
            }
            return EXIT_SUCCESS;
        }
        else
        {
            spdlog::error(options.error());
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

int App::encode(const whisp::EncodeConfig& config)
{
    spdlog::info("Running encode with config: {}", config);

    auto image = io::readImage(config.imageFile);
    std::span imageData{image.begin(), image.end()};

    io::FileReader fileReader{config.secretFile};
    CoderData dataToEncode{fileReader.read(), config.secretFile.filename()};
    const auto encodeResult = whisp::encode(config.algorithmConfig, imageData, dataToEncode);

    if (encodeResult.has_value())
    {
        const auto outputImage = config.imageFile.parent_path() /= ("encoded_" + config.imageFile.filename().string());
        image.save(outputImage.c_str());

        return EXIT_SUCCESS;
    }
    else
    {
        spdlog::error(encodeResult.error());
        return EXIT_FAILURE;
    }
}

int App::decode(const whisp::DecodeConfig& config)
{
    spdlog::info("Running decode with config: {}", config);

    auto image = io::readImage(config.imageFile);
    std::span span{image.begin(), image.end()};

    const auto decodeResult = whisp::decode(span);
    if (decodeResult.has_value())
    {
        const auto [data, filename] = decodeResult.value();
        const auto outputFile = config.imageFile.parent_path() /= filename;
        io::FileWriter fileWriter{outputFile};
        fileWriter.write(data);
        return EXIT_SUCCESS;
    }
    else
    {
        spdlog::error(decodeResult.error());
        return EXIT_FAILURE;
    }
    return EXIT_FAILURE;
}

}
