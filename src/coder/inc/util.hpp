#pragma once

#include "coder/inc/defs.hpp"
#include "util/types.hpp"
#include <cstdint>

namespace stegan
{
inline auto createMask(const int numBits)
{
    const std::uint8_t mask = (1 << numBits) - 1;
    return Byte{mask};
}
}
