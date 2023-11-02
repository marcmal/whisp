#pragma once

#include <filesystem>
#include <optional>
#include <string>
#include <variant>

namespace stegan
{
namespace config
{
struct Encode
{
    std::filesystem::path imageFilename;
    std::filesystem::path fileToEncode;
    int bitsPerChannel;
};

struct Decode
{
    std::filesystem::path imageFileName;
    int bitsPerChannel;
};

}

using Config = std::variant<config::Encode, config::Decode>;
}
