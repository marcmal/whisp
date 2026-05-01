module;

#include "cimg_wrapper.hpp"
#include <filesystem>
#include <vector>

export module whisp.io:image_reader;

export namespace whisp::io
{
struct ImageData
{
    std::vector<unsigned char> pixels;
    int width, height, channels, depth;
};

inline ImageData readImage(const std::filesystem::path& path)
{
    cimg_library::CImg<unsigned char> image(path.c_str());
    const int w = image.width();
    const int h = image.height();
    const int d = image.depth();
    const int c = image.spectrum();

    if (c == 3)
    {
        constexpr auto alphaChannelInitValue = 255;
        cimg_library::CImg<unsigned char> alpha(w, h, d, 1, alphaChannelInitValue);
        image = image.append(alpha, 'c');
    }

    ImageData data{};
    data.pixels.assign(image.begin(), image.end());
    data.width = image.width();
    data.height = image.height();
    data.channels = image.spectrum();
    data.depth = image.depth();
    return data;
}

inline void saveImage(const ImageData& data, const std::filesystem::path& path)
{
    cimg_library::CImg<unsigned char> image(data.width, data.height, 1, data.channels);
    std::copy(data.pixels.begin(), data.pixels.end(), image.begin());
    image.save(path.c_str());
}

}