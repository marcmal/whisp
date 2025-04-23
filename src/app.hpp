#pragma once

namespace whisp
{

struct EncodeConfig;
struct DecodeConfig;

class App
{
  public:
    // NOLINTNEXTLINE(cppcoreguidelines-avoid-c-arrays,modernize-avoid-c-arrays)
    int run(const int argc, const char* const argv[]);

  private:
    int encode(const whisp::EncodeConfig& config);
    int decode(const whisp::DecodeConfig& config);
};

}