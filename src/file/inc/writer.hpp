#pragma once

#include "util/types.hpp"
#include <filesystem>
#include <vector>

namespace fict_tele
{

class FileWriter
{
  public:
    explicit FileWriter(const std::filesystem::path& file);
    void write(const std::vector<fict_tele::Byte>& data);

  private:
    const std::filesystem::path file;
};

}
