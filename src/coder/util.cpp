module;

#include <cstdint>

module whisp.coder:util;
import whisp.util;

namespace whisp::coder
{
inline auto createMask(const int numBits)
{
    const std::uint8_t mask = (1 << numBits) - 1;
    return util::Byte{mask};
}

}
