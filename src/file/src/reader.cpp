#include "file/inc/reader.hpp"
#include <fstream>

namespace stegan
{

FileReader::FileReader(const std::filesystem::path& file) : file{file}
{
}

std::vector<Byte> FileReader::read() const
{
    std::ifstream fileStream(file, std::ios::in | std::ios::binary);
    return std::vector<stegan::Byte>{std::istreambuf_iterator(fileStream), {}};
}

}
