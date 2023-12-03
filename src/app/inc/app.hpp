#pragma once

namespace fict_tele
{

namespace config
{
struct Encode;
struct Decode;
}

class App
{
  public:
    int run(const int argc, const char* const argv[]); // NOLINT(cppcoreguidelines-avoid-c-arrays)

  private:
    void encode(const fict_tele::config::Encode& config);
    void decode(const fict_tele::config::Decode& config);
};

}