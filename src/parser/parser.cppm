module;

#include <expected>
#include <memory>

export module whisp.parser;

export import :config;
export import :error;

export namespace whisp::parser
{

class ArgParser
{
  public:
    ArgParser();
    ~ArgParser();
    // NOLINTNEXTLINE(cppcoreguidelines-avoid-c-arrays,modernize-avoid-c-arrays)
    [[nodiscard]] std::expected<Result, ParserError> parse(const int argc, const char* const argv[]);

  private:
    struct Impl;
    std::unique_ptr<Impl> pimpl;
};

}