#pragma once

#include "parser/argument_exception.hpp"
#include "parser/config.hpp"
#include <cxxopts.hpp>

namespace fict_tele
{
class CliParser
{
  public:
    CliParser();
    Config parse(const int argc,
                 const char* const argv[]); // NOLINT(cppcoreguidelines-avoid-c-arrays)

  private:
    void validateArgs(const cxxopts::ParseResult& parseResult) const;
    Config parse(const cxxopts::ParseResult& parseResult) const;

    cxxopts::Options options;
};
}
