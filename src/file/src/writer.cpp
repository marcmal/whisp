#include "file/inc/writer.hpp"
#include <fstream>

namespace stegan
{

FileWriter::FileWriter(const std::filesystem::path& file) : file{file}
{
}

void FileWriter::write(const std::vector<stegan::Byte>& data)
{
    std::ofstream fileStream(file, std::ios::out | std::ios::binary | std::ios::trunc);
    fileStream.write(reinterpret_cast<const char*>(data.data()), data.size());
}

}
