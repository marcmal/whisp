#include "io/image_reader.hpp"

namespace whisp::io
{

cimg_library::CImg<unsigned char> readImage(const std::filesystem::path& path)
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
        return image.append(alpha, 'c');
    }
    return image;
}

}