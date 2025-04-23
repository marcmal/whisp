#pragma once

#include "util/types.hpp"
#include <filesystem>
#include <vector>

namespace whisp::io
{

class FileWriter
{
  public:
    explicit FileWriter(const std::filesystem::path& file);
    void write(const std::vector<whisp::Byte>& data);

  private:
    std::filesystem::path file;
};

}
