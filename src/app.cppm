module;

export module whisp.app;

import whisp.parser;

export namespace whisp
{
class App
{
  public:
    // NOLINTNEXTLINE(cppcoreguidelines-avoid-c-arrays,modernize-avoid-c-arrays)
    int run(const int argc, const char* const argv[]);

  private:
    int encode(const parser::EncodeConfig& config);
    int decode(const parser::DecodeConfig& config);
};

}
