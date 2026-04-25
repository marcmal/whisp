#pragma once

#include "util/overload.hpp"
#include <variant>

namespace whisp
{

struct AlphaAlgorithmHeader
{
};

struct RgbAlgorithmHeader
{
    int bitsPerChannel;
};

using AlgorithmHeader = std::variant<AlphaAlgorithmHeader, RgbAlgorithmHeader>;

struct Header
{
    constexpr static auto ALPHA_MODE = 1;
    constexpr static auto RGB_MODE = 2;

    static auto createAlpha()
    {
        return Header{ALPHA_MODE, AlphaAlgorithmHeader{}};
    }

    static auto createRgb(const int bitsPerChannel)
    {
        return Header{RGB_MODE, RgbAlgorithmHeader{bitsPerChannel}};
    }

    std::size_t size() const
    {
        return std::visit(overload{
                              [](const AlphaAlgorithmHeader&) { return 1; },
                              [](const RgbAlgorithmHeader&) { return 2; },
                          },
                          header);
    }

    int mode;
    AlgorithmHeader header;
};

}
