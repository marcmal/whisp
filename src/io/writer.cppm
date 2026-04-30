module;

import whisp.util;
#include <filesystem>
#include <fstream>
#include <vector>

export module whisp.io:writer;

namespace whisp::io
{
using util::Byte;

export class FileWriter
{
  public:
    explicit FileWriter(const std::filesystem::path& file) : file{file}
    {
    }

    void write(const std::vector<Byte>& data)
    {
        std::ofstream fileStream(file, std::ios::out | std::ios::binary | std::ios::trunc);
        fileStream.write(
            reinterpret_cast<const char*>(data.data()), // NOLINT(cppcoreguidelines-pro-type-reinterpret-cast)
            static_cast<std::streamsize>(data.size()));
    }

  private:
    std::filesystem::path file;
};

}
