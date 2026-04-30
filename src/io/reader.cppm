module;

import whisp.util;
#include <filesystem>
#include <fstream>
#include <vector>

export module whisp.io:reader;

namespace whisp::io
{
using util::Byte;

export class FileReader
{
  public:
    explicit FileReader(const std::filesystem::path& file) : file{file}
    {
    }

    std::vector<Byte> read() const
    {
        std::ifstream fileStream(file, std::ios::in | std::ios::binary);
        return std::vector<Byte>{std::istreambuf_iterator(fileStream), {}};
    }

  private:
    std::filesystem::path file;
};

}
