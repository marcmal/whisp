#include "coder/decoder.hpp"
#include "coder/encode_exception.hpp"
#include "coder/encoder.hpp"
#include "file/inc/reader.hpp"
#include "file/inc/writer.hpp"
#include "parser/argument_exception.hpp"
#include "parser/cli_parser.hpp"
#include "util/overload.hpp"
#include <CImg.h>
#include <iostream>

void encode(const fict_tele::config::Encode& config)
{
    cimg_library::CImg<unsigned char> image(config.imageFilename.c_str());
    std::span span{image.begin(), image.end()};

    fict_tele::FileReader fileReader{config.fileToEncode};
    fict_tele::Encoder enc{span, config.bitsPerChannel};
    enc.encode(fileReader.read());

    const auto outputImage = "encoded_" + config.imageFilename.string();
    image.save(outputImage.c_str());
}

void decode(const fict_tele::config::Decode& config)
{
    cimg_library::CImg<unsigned char> image(config.imageFileName.c_str());
    std::span span{image.begin(), image.end()};

    fict_tele::FileWriter fileWriter{"output.txt"};
    fict_tele::Decoder dec{span, config.bitsPerChannel};
    fileWriter.write(dec.decode());
}

int main(int argc, char* argv[])
{
    try
    {
        const auto options = fict_tele::CliParser{}.parse(argc, argv);
        std::visit(overload{[](const fict_tele::config::Encode& config) { encode(config); },
                            [](const fict_tele::config::Decode& config) { decode(config); }},
                   options);
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
}
