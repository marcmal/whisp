#pragma once

#include "parser/config.hpp"
#include "parser/error.hpp"
#include <CLI/CLI.hpp>
#include <expected>

namespace whisp
{
using Result = std::expected<ParserResult, ParserError>;

class ArgParser
{
  public:
    // NOLINTNEXTLINE(cppcoreguidelines-avoid-c-arrays,modernize-avoid-c-arrays)
    [[nodiscard]] Result parse(const int argc, const char* const argv[]);

  private:
    CLI::App app{"Whisp - image steganography tool"};
};
}
