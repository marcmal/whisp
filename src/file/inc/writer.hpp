#pragma once

#include "util/types.hpp"
#include <filesystem>
#include <vector>

namespace stegan
{

class FileWriter
{
  public:
    explicit FileWriter(const std::filesystem::path& file);
    void write(const std::vector<stegan::Byte>& data);

  private:
    const std::filesystem::path file;
};

}
