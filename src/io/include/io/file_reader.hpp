#pragma once

#include "util/types.hpp"
#include <filesystem>
#include <vector>

namespace whisp::io
{

class FileReader
{
  public:
    explicit FileReader(const std::filesystem::path& file);
    [[nodiscard]] std::vector<Byte> read() const;

  private:
    std::filesystem::path file;
};

}
