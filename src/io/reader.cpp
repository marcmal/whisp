#include "io/file_reader.hpp"
#include <fstream>

namespace whisp::io
{

FileReader::FileReader(const std::filesystem::path& file) : file{file}
{
}

std::vector<Byte> FileReader::read() const
{
    std::ifstream fileStream(file, std::ios::in | std::ios::binary);
    return std::vector<whisp::Byte>{std::istreambuf_iterator(fileStream), {}};
}

}
