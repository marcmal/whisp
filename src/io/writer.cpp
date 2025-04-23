#include "io/file_writer.hpp"
#include <fstream>

namespace whisp::io
{

FileWriter::FileWriter(const std::filesystem::path& file) : file{file}
{
}

void FileWriter::write(const std::vector<whisp::Byte>& data)
{
    std::ofstream fileStream(file, std::ios::out | std::ios::binary | std::ios::trunc);
    fileStream.write(reinterpret_cast<const char*>(data.data()), // NOLINT(cppcoreguidelines-pro-type-reinterpret-cast)
                     static_cast<std::streamsize>(data.size()));
}

}
