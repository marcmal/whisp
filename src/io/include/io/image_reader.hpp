#pragma once

#include <filesystem>
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wold-style-cast"
#include <CImg.h>
#pragma GCC diagnostic pop

namespace whisp::io
{

cimg_library::CImg<unsigned char> readImage(const std::filesystem::path& path);

}