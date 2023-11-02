#include "coder/inc/decoder.hpp"
#include "coder/inc/encode_exception.hpp"
#include "coder/inc/encoder.hpp"
#include "file/inc/reader.hpp"
#include "file/inc/writer.hpp"
#include "parser/inc/argument_exception.hpp"
#include "parser/inc/cli_parser.hpp"
#include "util/overload.hpp"
#include <CImg.h>

void encode(const stegan::config::Encode& config)
{
    cimg_library::CImg<unsigned char> image(config.imageFilename.c_str());
    std::span span{image.begin(), image.end()};

    stegan::FileReader fileReader{config.fileToEncode};
    stegan::Encoder enc{span, config.bitsPerChannel};
    enc.encode(fileReader.read());

    const auto outputImage = "encoded_" + config.imageFilename.string();
    image.save(outputImage.c_str());
}

void decode(const stegan::config::Decode& config)
{
    cimg_library::CImg<unsigned char> image(config.imageFileName.c_str());
    std::span span{image.begin(), image.end()};

    stegan::FileWriter fileWriter{"output.txt"};
    stegan::Decoder dec{span, config.bitsPerChannel};
    fileWriter.write(dec.decode());
}

int main(int argc, char* argv[])
{
    try
    {
        const auto options = stegan::CliParser{}.parse(argc, argv);
        std::visit(overload{[](const stegan::config::Encode& config) { encode(config); },
                            [](const stegan::config::Decode& config) { decode(config); }},
                   options);
    }
    catch (const stegan::ArgumentException& e)
    {
        std::cout << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    catch (const stegan::EncodeException& e)
    {
        std::cout << e.what() << std::endl;
        return EXIT_FAILURE;
    }
}
