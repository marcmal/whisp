#include "app.hpp"
#include "coder/decoder.hpp"
#include "coder/encode_exception.hpp"
#include "coder/encoder.hpp"
#include "file/reader.hpp"
#include "file/writer.hpp"
#include "parser/argument_exception.hpp"
#include "parser/cli_parser.hpp"
#include "util/overload.hpp"
#include <CImg.h>
#include <cxxopts.hpp>
#include <filesystem>
#include <iostream>

namespace fict_tele
{

int App::run(const int argc, const char* const argv[]) // NOLINT(cppcoreguidelines-avoid-c-arrays)
{
    try
    {
        const auto options = fict_tele::CliParser{}.parse(argc, argv);
        std::visit(overload{[this](const fict_tele::config::Encode& config) { encode(config); },
                            [this](const fict_tele::config::Decode& config) { decode(config); }},
                   options);
        return EXIT_SUCCESS;
    }
    catch (const fict_tele::ArgumentException& e)
    {
        std::cout << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    catch (const fict_tele::EncodeException& e)
    {
        std::cout << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    catch (const std::exception& e)
    {
        std::cout << "Unknown exception: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
}

void App::encode(const fict_tele::config::Encode& config)
{
    cimg_library::CImg<unsigned char> image(config.imageFilename.c_str());
    std::span imageData{image.begin(), image.end()};

    fict_tele::FileReader fileReader{config.fileToEncode};
    fict_tele::Encoder enc{imageData, config.bitsPerChannel};
    enc.encode(fileReader.read(), config.fileToEncode.filename());

    const auto outputImage = config.imageFilename.parent_path() /=
        ("encoded_" + config.imageFilename.filename().string());
    image.save(outputImage.c_str());
}

void App::decode(const fict_tele::config::Decode& config)
{
    cimg_library::CImg<unsigned char> image(config.imageFileName.c_str());
    std::span span{image.begin(), image.end()};

    fict_tele::Decoder dec{span, config.bitsPerChannel};
    const auto [filename, data] = dec.decode();

    const auto outputFile = config.imageFileName.parent_path() /= filename;
    fict_tele::FileWriter fileWriter{outputFile};
    fileWriter.write(data);
}

}