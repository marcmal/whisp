#pragma once

#include "util/types.hpp"
#include <cstdint>

namespace whisp
{

inline auto createMask(const int numBits)
{
    const std::uint8_t mask = (1 << numBits) - 1;
    return Byte{mask};
}

}
