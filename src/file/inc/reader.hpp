#pragma once

#include "util/types.hpp"
#include <filesystem>
#include <vector>

namespace stegan
{

class FileReader
{
  public:
    explicit FileReader(const std::filesystem::path& file);
    std::vector<Byte> read() const;

  private:
    const std::filesystem::path file;
};

}
