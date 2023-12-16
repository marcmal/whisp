#pragma once

#include "coder/defs.hpp"
#include "util/types.hpp"
#include <cstdint>

namespace fict_tele
{

inline auto createMask(const int numBits)
{
    const std::uint8_t mask = (1 << numBits) - 1;
    return Byte{mask};
}

}
