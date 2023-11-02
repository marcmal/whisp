#pragma once

#include "parser/inc/argument_exception.hpp"
#include "parser/inc/config.hpp"
#include <cxxopts.hpp>
#include <optional>

namespace stegan
{
class CliParser
{
  public:
    CliParser();
    Config parse(const int argc, const char* const argv[]);

  private:
    void validateArgs(const cxxopts::ParseResult& parseResult) const;
    Config parse(const cxxopts::ParseResult& parseResult) const;

    cxxopts::Options options;
};
} // namespace stegan
